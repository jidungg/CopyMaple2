#include "stdafx.h"
#include "NPC.h"
#include "ModelObject.h"
#include "GameInstance.h"
#include "NPCDataBase.h"
#include "Client_Utility.h"
#include "StateMachine.h"
#include "Collider_Sphere.h"
#include "Player.h"
#include "UIBundle.h"
#include "QuestDataBase.h"

CNPC::CNPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInteractableObject(pDevice, pContext)
{
}


CNPC::CNPC(const CNPC& Prototype)
	: CInteractableObject(Prototype)
{
}



HRESULT CNPC::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	tDesc.vCenter = { 0,0.5,0, };
	tDesc.fRadius = 1.f;
	m_vecCollider.resize(1);
	m_vecCollider[0] = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));
	Add_Component(m_vecCollider[0], L"Com_Collider_Body");
	

	NPC_DESC* pDesc = (NPC_DESC*)pArg;
	m_pNPCData =  NPCDB->Get_Data(pDesc->eNPCId);

	//Body
	CModelObject::MODELOBJ_DESC tModelDesc = {};
	tModelDesc.eModelProtoLevelID = Get_CurrentTrueLevel();
	lstrcpyW(tModelDesc.szModelProtoName, m_pNPCData->szModelTag);
	m_pBody = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	Add_Child(m_pBody);
	m_pBody->Get_Transform()->Scaling(1.2f, 1.2f, 1.2f);

	if (FAILED(Ready_AnimStateMachine()))
		return E_FAIL;
	return S_OK;
}
void CNPC::Update(_float fTimeDelta)
{
	m_fBoreTimeAcc += fTimeDelta;
	if (m_fBoreTimeAcc >= m_fBoreTime)
	{
		m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_BORE_TRIGGER);
	}
	__super::Update(fTimeDelta);
	for (auto& pCollider : m_vecCollider)
	{
		pCollider->Update(XMLoadFloat4x4(&m_WorldMatrix));
	}

}
void CNPC::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}
HRESULT CNPC::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}
const ConversationNodeData& CNPC::Get_ConversationData(_int iIdx)
{
	return m_pNPCData->mapChat[iIdx];
}
void CNPC::To_NextConversation(_uint iSelectedOptionIdx)
{
	//옵션이 없으면
	if (0 == m_pNPCData->mapChat[m_iConversationIndex].vecOption.size())
		m_iConversationIndex = m_pNPCData->mapChat[m_iConversationIndex].iNextNode;
	//옵션이 있으면
	else
	{
		const ChatOptionData tOptionData = Get_ConversationData(m_iConversationIndex).vecOption[iSelectedOptionIdx];
		//퀘스트 옵션
		if (tOptionData.eOptType == CHAT_OPT_TYPE::QUEST)
		{
			//퀘스트 대화 시작
			QuestData* pQuestData = QUESTDB->Get_Data(tOptionData.eQuestID);
			m_iConversationIndex = pQuestData->Get_AccurateConversationIndex();
			m_eCurrentConversationQuest = tOptionData.eQuestID;
		}
		//퀘스트 포기 옵션
		else if (tOptionData.eOptType == CHAT_OPT_TYPE::ABANDON_QUEST)
		{
			m_bAbandonQuest = true;
			m_iConversationIndex = tOptionData.iNextNode;
		}
		//노말 옵션
		else
			m_iConversationIndex = tOptionData.iNextNode;
	}

	//마지막이면
	if (m_iConversationIndex == -1)
	{
		End_Conversation();
	}
	//마지막이 아니면
	else
	{
		m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_TALK_TRIGGER);
		UIBUNDLE->Set_NPCDialogData(Get_ConversationData(m_iConversationIndex));
	}

}
void CNPC::End_Conversation()
{
	if (m_eCurrentConversationQuest != QUEST_ID::LAST)
	{
		if (m_bAbandonQuest)
			QUESTDB->Abandon_Quest(m_eCurrentConversationQuest);
		else
		{
			QuestData* pQuestData = QUESTDB->Get_Data(m_eCurrentConversationQuest);
			QUEST_STATE eState = pQuestData->eState;
			switch (eState)
			{
			case Client::QUEST_STATE::NOT_ACCEPTED:
				QUESTDB->Accept_Quest(m_eCurrentConversationQuest);
				break;
			case Client::QUEST_STATE::ACCEPTED:
				if (QUESTDB->Is_SatisfiedAcceptCondition(pQuestData->eQuestID))
					QUESTDB->Complete_Quest(m_eCurrentConversationQuest);
				break;
			case Client::QUEST_STATE::COMPLETED:
			case Client::QUEST_STATE::LAST:
			default:
				break;
			}
		}
		m_eCurrentConversationQuest = QUEST_ID::LAST;
		m_bAbandonQuest = false;
	}
	m_iConversationIndex = -1;
	UIBUNDLE->Set_NPCDialogActive(false);
	UIBUNDLE->Set_HUDActive(true);
}
HRESULT CNPC::Ready_AnimStateMachine()
{
	//AnimStateMachine
	m_pAnimStateMachine = static_cast<CStateMachine*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Prototype_GameObject_StateMachine")));
	if (nullptr == m_pAnimStateMachine) return E_FAIL;
	Add_Component(m_pAnimStateMachine, TEXT("Com_StateMachine"));

	m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CNPC::On_StateChange, this, placeholders::_1));
	m_pAnimStateMachine->Register_OnSubStateChangeCallBack(bind(&CNPC::On_SubStateChange, this, placeholders::_1));
	m_pBody->Register_OnAnimEndCallBack(bind(&CNPC::On_AnimEnd, this, placeholders::_1));

	m_pBody->Set_AnimationLoop(m_pNPCData->mapAnimIdx[N_AS_IDLE].front(), true);


	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_ANIM_END_TRIGGER));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_TALK_TRIGGER));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_BORE_TRIGGER));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_EMOTION_TRIGGER));
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_MOVE),&m_bMove);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_WALK),&m_bWalk);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_RANDOM),&m_fRandom);

	m_pAnimStateMachine->Add_State(N_BS_IDLE);
	m_pAnimStateMachine->Add_State(N_BS_MOVE);
	m_pAnimStateMachine->Add_State(N_BS_TALK);
	m_pAnimStateMachine->Add_State(N_BS_BORE);
	m_pAnimStateMachine->Add_State(N_BS_EMOTION);
	for (auto& vecAnimIdx : m_pNPCData->mapAnimIdx)
	{
		for (auto& iAnimIdx : vecAnimIdx.second)
			m_pAnimStateMachine->Add_State(iAnimIdx);
	}

	CTransition* pTransition;
	//MAINTRANSITION
	pTransition = m_pAnimStateMachine->Add_Transition(N_BS_IDLE, N_BS_TALK);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_TALK_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(N_BS_TALK, N_BS_IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);

	pTransition = m_pAnimStateMachine->Add_Transition(N_BS_IDLE, N_BS_BORE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_BORE_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(N_BS_BORE, N_BS_IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);

	pTransition = m_pAnimStateMachine->Add_Transition(N_BS_IDLE, N_BS_EMOTION);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_EMOTION_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(N_BS_EMOTION, N_BS_IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);

	pTransition = m_pAnimStateMachine->Add_Transition(N_BS_IDLE, N_BS_MOVE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(N_BS_MOVE, N_BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, false);

	//SUBTRANSITION
	pTransition = m_pAnimStateMachine->Add_SubTransition(N_BS_BORE, m_pNPCData->mapAnimIdx[N_AS_BORE].front());
	vector<_uint>& vecAnimIndex = m_pNPCData->mapAnimIdx[N_AS_BORE];
	_uint iCount = vecAnimIndex.size();
	_float fRandomStride = 1.f / (_float)iCount;
	_float fRandomStart = 0.f;
	_float fRandomEnd = fRandomStride;
	for (_uint idx = 0; idx < iCount - 1; idx++)
	{
		pTransition = m_pAnimStateMachine->Add_SubTransition(N_BS_BORE, vecAnimIndex[idx]);
		m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
		m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::EQUAL_GREATER,fRandomStart);
		m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::EQUAL_LESS,fRandomEnd);
		fRandomStart += fRandomStride;
		fRandomEnd += fRandomStride;
	}
	pTransition = m_pAnimStateMachine->Add_SubTransition(N_BS_IDLE, m_pNPCData->mapAnimIdx[N_AS_IDLE].front());

	pTransition = m_pAnimStateMachine->Add_SubTransition(N_BS_TALK, m_pNPCData->mapAnimIdx[N_AS_TALK].front());

	pTransition = m_pAnimStateMachine->Add_SubTransition(N_BS_MOVE, m_pNPCData->mapAnimIdx[N_AS_RUN].front());
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WALK, CONDITION_TYPE::EQUAL, false);

	pTransition = m_pAnimStateMachine->Add_SubTransition(N_BS_MOVE, m_pNPCData->mapAnimIdx[N_AS_WALK].front());
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WALK, CONDITION_TYPE::EQUAL, true);



	m_pAnimStateMachine->Set_CurrentState(N_BS_IDLE);
	return S_OK;
}
void CNPC::On_StateChange(_uint iState)
{
}
void CNPC::On_SubStateChange(_uint iSubState)
{
	m_fBoreTimeAcc = 0;
	m_pBody->Switch_Animation(iSubState);
}
void CNPC::On_AnimEnd(_uint iAnimIdx)
{
	m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_ANIM_END_TRIGGER);

}
void CNPC::Interact(CPlayer* pActor)
{
	if (-1 == m_iConversationIndex)
	{
		m_pTransformCom->LookAt(pActor->Get_WorldPosition());
		m_pAnimStateMachine->Set_CurrentState((_uint)N_BS_TALK, m_pNPCData->mapAnimIdx[N_AS_TALK].front());
		m_iConversationIndex = 0;
		UIBUNDLE->Set_HUDActive(false);
		UIBUNDLE->Set_NPCDialogActive(true);
		UIBUNDLE->Set_NPCDialogNPC(this);
		UIBUNDLE->Set_NPCDialogData(Get_ConversationData(m_iConversationIndex));
	}

}

_bool CNPC::Is_InteractionPossible(CPlayer* pActor)
{
	return true;
}
CNPC* CNPC::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNPC* pInstance = new CNPC(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CNPC::Clone(void* pArg)
{
	CNPC* pInstance = new CNPC(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CNPC::Free()
{
	__super::Free();
	Safe_Release(m_pBody);
	Safe_Release(m_pAnimStateMachine);
}



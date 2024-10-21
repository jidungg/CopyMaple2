#include "stdafx.h"
#include "Player.h"
#include "Client_Utility.h"
#include "ModelObject.h"
#include "GameInstance.h"
#include "StateMachine.h"


CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CPawn(Prototype)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Body()))
		return E_FAIL;
	if (FAILED(Ready_AnimStateMachine()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Body()
{
	//Body
	CModelObject::MODELOBJ_DESC tModelDesc;
	tModelDesc.fRotationPerSec = 5.f;
	tModelDesc.fSpeedPerSec = 1.f;
	tModelDesc.eModelType = CModel::TYPE_ANIM;
	tModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tModelDesc.strModelProtoName, "Prototype_Model_Player");
	tModelDesc.eShaderProtoLevelID = LEVEL_LOADING;
	strcpy_s(tModelDesc.strShaderProtoName, "Prototype_Component_Shader_VtxAnimMesh");
	m_pBody = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	Add_Child(m_pBody);
	m_pBody->Switch_Animation((_uint)ANIM_STATE::idle);
	m_pBody->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(0, 1, 0, 1));

	return S_OK;
}

HRESULT CPlayer::Ready_AnimStateMachine()
{
	//AnimStateMachine
	m_pAnimStateMachine = static_cast<CStateMachine*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Prototype_GameObject_StateMachine")));
	Add_Component(m_pAnimStateMachine, TEXT("Com_StateMachine"));
	m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CPlayer::On_StateChange, this, placeholders::_1));

	CState* pState; CTransition* pTransition; Condition* pCondition;

	//CONDITION
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_ANIMEND));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_ATTACK));
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_UPFORCE), &m_fUpForce);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_HEIGHT), &m_fHeight);

	//STATE
	for (_uint i = 0; i < (_uint)ANIM_STATE::LAST; i++)
		m_pAnimStateMachine->Add_State(i);

	m_pBody->Set_AnimationLoop((_uint)ANIM_STATE::idle, true);
	m_pAnimStateMachine->Set_CurrentState((_uint)ANIM_STATE::idle);

	//TRAINSITION
	//idle -> attack
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::idle, (_uint)ANIM_STATE::attack_01);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::AC_ATTACK);
	//idle->Jump
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::idle, (_uint)ANIM_STATE::jump_up_a);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::AC_UPFORCE, CONDITION_TYPE::CT_EQUAL_GREATER, m_fJumpPower-1.f);
	//attack->idle
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::attack_01, (_uint)ANIM_STATE::idle);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::AC_ANIMEND);

	//jump_up_a->jump_down_a
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::jump_up_a, (_uint)ANIM_STATE::jump_down_a);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::AC_UPFORCE, CONDITION_TYPE::CT_EQUAL_LESS, 0.f);

	//jump_down_a->jump_land
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::jump_down_a, (_uint)ANIM_STATE::jump_land);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::AC_HEIGHT, CONDITION_TYPE::CT_EQUAL_LESS, 1.f);

	//jump_land -> idle
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::jump_land, (_uint)ANIM_STATE::idle);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::AC_ANIMEND);

	return S_OK;
}
void CPlayer::Update(_float fTimeDelta)
{
	if(m_pBody->Is_AnimEnd())
		m_pAnimStateMachine->Trigger_ConditionVariable((_uint)ANIM_CONDITION::AC_ANIMEND);
	m_fUpForce -= fTimeDelta * 9.8f;
	m_fHeight += m_fUpForce * fTimeDelta;
	if(m_fHeight <= 1.f)
	{
		m_fUpForce = 0;
		m_fHeight = 1.f;
	}
	__super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND,this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active() && child->Is_Dead() == false)
			child->Render();
	}
	return S_OK;
}


void CPlayer::Receive_KeyInput(KEY eKey, KEY_STATE eKeyState, _float fTimeDelta)
{
	XMVECTOR vDir = XMVectorSet(0, 0, 0, 0);

	if (eKey == KEY::RIGHT && eKeyState == KEY_STATE::PRESSING)
		vDir += Get_Direction_Vector(DIR_E);
	if (eKey == KEY::UP && eKeyState == KEY_STATE::PRESSING)
		vDir += Get_Direction_Vector(DIR_N);
	if (eKey == KEY::DOWN && eKeyState == KEY_STATE::PRESSING)
		vDir += Get_Direction_Vector(DIR_S);
	if (eKey == KEY::LEFT && eKeyState == KEY_STATE::PRESSING)
		vDir += Get_Direction_Vector(DIR_W);
	m_pTransformCom->Go_Direction(vDir, fTimeDelta);

	if (eKey == KEY::X && eKeyState == KEY_STATE::DOWN)
	{
			m_pAnimStateMachine->Trigger_ConditionVariable((_uint)ANIM_CONDITION::AC_ATTACK);
	}
	if (eKey == KEY::C && eKeyState == KEY_STATE::DOWN)
	{
		m_fUpForce = m_fJumpPower;
	}
}

void CPlayer::On_StateChange(_uint iState)
{
	m_pBody->Switch_Animation(iState);

}



CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pAnimStateMachine);
}

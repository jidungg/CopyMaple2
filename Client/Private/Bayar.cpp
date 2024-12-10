#include "stdafx.h"
#include "Bayar.h"
#include "ModelObject.h"
#include "GameInstance.h"
#include "AttachableBodyPart.h"
#include "Collider_Sphere.h"
#include "StateMachine.h"
#include "SkillManager.h"
#include "CubeTerrain.h"
#include "Client_Utility.h"
#include "WayFinder.h"

CBayar::CBayar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
	m_vecCollider.resize(1);

}

CBayar::CBayar(const CBayar& Prototype)
	: CMonster(Prototype)
{
	m_vecCollider.resize(1);
}

HRESULT CBayar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBayar::Initialize(void* pArg)
{
	CMonster::MONSTER_DESC* pDesc = static_cast<CMonster::MONSTER_DESC*>(pArg);
	pDesc->fRotationPerSec = XMConvertToRadians(360.f);
	pDesc->fSpeedPerSec = 5.f;
	pDesc->iBodyColliderIndex = PART_ID::BODY;
	pDesc->iColliderCount = PART_LAST;
	pDesc->eMonID = MONSTER_ID::BAYAR;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_AttacableParts(pDesc)))
		return E_FAIL;

	m_pTransformCom->Scaling(2.f, 2.f, 2.f);

	return S_OK;
}

HRESULT CBayar::Ready_AttacableParts(CHARACTER_DESC* pDesc)
{
	m_vecAttachablePart.resize(PART_ID::PART_LAST);

	CAttachableBodyPart::ATTACHABLE_BODYPART_DESC tDesc = {};
	tDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	CCollider_Sphere::SPHERE_COLLIDER_DESC tColliderDesc = {};

	//BODY
	tColliderDesc.fRadius = 0.5;
	tColliderDesc.vCenter = { 0,0.5, 0 };
	tDesc.tColliderDesc = tColliderDesc;
	tDesc.pSocketMatrix = m_pBody->Get_BoneMatrix("Back_Bone01");
	m_vecAttachablePart[PART_ID::BODY] = static_cast<CAttachableBodyPart*>(m_pGameInstance->Clone_Proto_Object_Stock(CAttachableBodyPart::m_szProtoTag, &tDesc));
	Safe_AddRef(m_vecAttachablePart[PART_ID::BODY]);
	m_pGameInstance->Add_GameObject_ToLayer(Get_CurrentTrueLevel(), LAYER_INTERACTION, m_vecAttachablePart[PART_ID::BODY]);

	//LEFTARM
	tColliderDesc.fRadius = 0.4;
	tColliderDesc.vCenter = { 0,0, 0 };
	tDesc.tColliderDesc = tColliderDesc;
	tDesc.pSocketMatrix = m_pBody->Get_BoneMatrix("02030008_N_SandstoneGiant22.001");
	m_vecAttachablePart[PART_ID::LEFT_ARM] = static_cast<CAttachableBodyPart*>(m_pGameInstance->Clone_Proto_Object_Stock(CAttachableBodyPart::m_szProtoTag, &tDesc));
	Safe_AddRef(m_vecAttachablePart[PART_ID::LEFT_ARM]);
	m_pGameInstance->Add_GameObject_ToLayer(Get_CurrentTrueLevel(), LAYER_INTERACTION, m_vecAttachablePart[PART_ID::LEFT_ARM]);


	//RIGHTARM
	tColliderDesc.fRadius = 0.4;
	tColliderDesc.vCenter = { 0,0, 0 };
	tDesc.pSocketMatrix = m_pBody->Get_BoneMatrix("02030008_N_SandstoneGiant24.001");
	m_vecAttachablePart[PART_ID::RIGHT_ARM] = static_cast<CAttachableBodyPart*>(m_pGameInstance->Clone_Proto_Object_Stock(CAttachableBodyPart::m_szProtoTag, &tDesc));
	Safe_AddRef(m_vecAttachablePart[PART_ID::RIGHT_ARM]);
	m_pGameInstance->Add_GameObject_ToLayer(Get_CurrentTrueLevel(), LAYER_INTERACTION, m_vecAttachablePart[PART_ID::RIGHT_ARM]);


	return S_OK;
}
HRESULT CBayar::Ready_AnimStateMachine()
{
	//TODO: JUMP, DEADDAMG. DEADIDLE 추가해야함.
	m_pBody->Set_Animation((_uint)STATE::AS_REGEN);

	m_pBody->Set_AnimationLoop((_uint)STATE::AS_WALK_A, true);
	m_pBody->Set_AnimationLoop((_uint)STATE::AS_RUN, true);
	m_pBody->Set_AnimationLoop((_uint)STATE::AS_IDLE, true);
	m_pBody->Set_AnimationLoop((_uint)STATE::AS_ATTACK_IDLE, true);
	m_pBody->Set_AnimationLoop((_uint)STATE::AS_DEAD_IDLE_A, true);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_A, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_B, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_C, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_D, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_E, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_F, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_G, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_H, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_A, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_B, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_C, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_D, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_E, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_G, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_H, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_3_E, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_3_G, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_3_H, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_4_G, 1);



	return __super::Ready_AnimStateMachine();
}
void CBayar::On_AnimEnd(_uint iAnimIdx)
{
	if (iAnimIdx == (_uint)STATE::AS_ATTACK_2_G && m_iGatheringCount < m_iMaxGatheringCount)
	{
		m_pBody->Switch_Animation((_uint)STATE::AS_ATTACK_2_G);
		m_iGatheringCount++;
	}
	else
	{
		m_iGatheringCount = 0;
		__super::On_AnimEnd(iAnimIdx);
	}

}

_bool CBayar::Is_Attached(CPlayer* pPlayer)
{
	for (auto& pAttachable : m_vecAttachablePart)
	{
		if(pAttachable->Is_Attached(pPlayer)) return true;
	}
	return false;
}

void CBayar::To_NextSkill()
{
	//m_iCurrentSkillID = (_int)SKILL_ID::BAYAR_PALM_STRIKE;
	__super::To_NextSkill();
}
_bool CBayar::FindWay(_vector& vStart, _vector& vGoal, _uint iSearchRange)
{
	return m_pWayFinder->FindWay(vStart, vGoal, m_iSearchRange,true);
}
void CBayar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	m_bOnFloor = true;
	m_fUpForce = 0;
}

void CBayar::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}


void CBayar::Update_Collider()
{
	//_matrix		SocketMatrix;
	//for (_uint i = 0; i < PART_LAST; i++)
	//{

	//	if (i == DETECTION)
	//		m_vecCollider[i]->Update(m_pTransformCom->Get_WorldMatrix());
	//	else
	//	{
	//		SocketMatrix = XMLoadFloat4x4(m_vecPartsMatrix[i]);
	//		for (size_t i = 0; i < 3; i++)
	//			SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);
	//		m_vecCollider[i]->Update(SocketMatrix * m_pTransformCom->Get_WorldMatrix());
	//	}
	//}
	__super::Update_Collider();
}

_bool CBayar::Check_Collision(CGameObject* pOther)
{
	return __super::Check_Collision(pOther);
}
void CBayar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}




HRESULT CBayar::Render()
{
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active() && child->Is_Dead() == false)
			child->Render();
	}
	//for (auto& pCollider : m_vecCollider)
	//{
	//	if (pCollider->Is_Active())
	//		pCollider->Render();
	//}
	return S_OK;
}






CBayar* CBayar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBayar* pInstance = new CBayar(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBayar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBayar::Clone(void* pArg)
{
	CBayar* pInstance = new CBayar(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBayar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayar::Free()
{
	__super::Free();
	for (auto& pAttachable : m_vecAttachablePart)
	{
		Safe_Release(pAttachable);
	}
	m_vecAttachablePart.clear();
}


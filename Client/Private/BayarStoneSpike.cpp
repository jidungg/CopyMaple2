#include "stdafx.h"
#include "BayarStoneSpike.h"
#include "Bullet_BayarStoneSpike.h"
#include "Collider_Sphere.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Client_Utility.h"
#include "CubeTerrain.h"

_int dX[5] = { 0,1,0,-1,0 };
_int dZ[5] = { 0,0,1,0,-1 };

CBayarStoneSpike::CBayarStoneSpike()
	:CSkill()
{
}

HRESULT CBayarStoneSpike::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	//TargetSeracher
	CCollider_Sphere::SPHERE_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fRadius = 5.f;
	tTargetSearcherDesc.vCenter = { 0.f, 0.4f, 0.f };
	m_pTargetSearcher = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);
	if (nullptr == m_pTargetSearcher)
		return E_FAIL;

	//Bullet
	CBullet::BULLET_DESC tBulletDesc;
	tBulletDesc.pShooter = m_pUser;
	tBulletDesc.szHitEffectTag = "eff_sandstonebiggiant_attack_02_b_hit.effmodel";
	CBullet_BayarStoneSpike* pBullet = static_cast<CBullet_BayarStoneSpike*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_BayarStoneSpike::m_szProtoTag, &tBulletDesc));
	pBullet->Set_Active(false);
	m_pBulletPool = CObjectPool<CBullet_BayarStoneSpike>::Create(pBullet, &tBulletDesc,25);
	if (nullptr == m_pBulletPool)
		return E_FAIL;


	return S_OK;
}

void CBayarStoneSpike::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	m_pBulletPool->Update_LentObject(fTimeDelta);
}

void CBayarStoneSpike::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pBulletPool->LateUpdate_LentObject(fTimeDelta);

	if (false == m_bAttack)
		return;
	if (m_fTimeAcc >= m_fAttackInterval)
	{
		m_fTimeAcc = 0.f;
		m_pTargetSearcher->Update(m_pUser->Get_Transform()->Get_WorldMatrix());
		list<CGameObject*> listTarget;
		SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);

		for (auto& pTarget : listTarget)
		{
			Launch_Bullets(pTarget->Get_Position());

		}

	}
	m_fTimeAcc += fTimeDelta;

}

void CBayarStoneSpike::On_Cast()
{
}

void CBayarStoneSpike::On_CastingEnd()
{
}

void CBayarStoneSpike::Fire()
{
	m_bAttack = true;
}

void CBayarStoneSpike::On_AttackEnd()
{
	m_bAttack = false;
}

void CBayarStoneSpike::Launch_Bullets(_vector vTargetPos)
{
	m_pBulletPool->Retrive_Object([](CBullet_BayarStoneSpike* pObj) ->_bool
		{
			return pObj->Is_Valid();
		});
	m_pTerrain = static_cast<CCubeTerrain*>(m_pGameInstance->Get_FirstGameObject(Get_CurrentTrueLevel(), LAYERID::LAYER_TERRAIN));

	_vector vTargetFloorPos = vTargetPos;
	_float fFloorHeight = m_pTerrain->Get_FloorHeight(vTargetPos);
	if (fFloorHeight < 0) return;

	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
	fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;

	vTargetFloorPos= XMVectorSetY( vTargetFloorPos, fFloorHeight);
	_uint iCenterIndex =  m_pTerrain->PosToIndex(vTargetFloorPos);
	_vector vCenterPos = m_pTerrain->Get_ContainedCellPosition(vTargetFloorPos);

	CBullet_BayarStoneSpike* pBullet = m_pBulletPool->Get_Object();
	pBullet->Launch(fDmg, vCenterPos);

	XMUINT3 i3CenterIdex = m_pTerrain->SplitIndex(iCenterIndex);

	for (_uint i = 0; i < 5; i++)
	{
		XMUINT3 i3CurrentIdx = i3CenterIdex;
		i3CurrentIdx.x += dX[i];
		i3CurrentIdx.z += dZ[i];
		if (m_pTerrain->Is_ValidIndex(i3CurrentIdx) == false)
			continue;
		_uint iCurrentIndex = m_pTerrain->CombineIndex(i3CurrentIdx);
		_vector vCurrentPos = m_pTerrain->IndexToPos(iCurrentIndex);
		pBullet = m_pBulletPool->Get_Object();
		pBullet->Launch(fDmg, vCurrentPos);
	}
	
}

CBayarStoneSpike* CBayarStoneSpike::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBayarStoneSpike* pInstance = new CBayarStoneSpike;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBayarStoneSpike Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayarStoneSpike::Free()
{
	__super::Free();
	Safe_Release(m_pBulletPool);
}

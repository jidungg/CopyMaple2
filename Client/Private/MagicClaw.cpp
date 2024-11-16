#include "stdafx.h"
#include "MagicClaw.h"
#include "Collider_Frustum.h"
#include "Collider_AABB.h"
#include "GameInstance.h"
#include "Character.h"
#include "Collider.h"
#include "MeshCollider.h"
#include "Player.h"
#include "Bullet.h"
#include "Bullet_MagicClaw.h"

CMagicClaw::CMagicClaw()
    :CSkill()
{

}

void CMagicClaw::Initialzie_AnimEvent()
{
	for (auto& eventTime : m_pSkillDesc->mapAnimEventTime)
	{
		_uint iAnimIdx = m_pSkillDesc->vecAnimation[eventTime.first];
		ANIM_EVENT tAnimEvent;
		tAnimEvent.fTime = eventTime.second;
		tAnimEvent.pFunc = std::bind(&CMagicClaw::AnimEventFunc1, this);
		m_mapAnimEvent.insert({ iAnimIdx, tAnimEvent });
	}
}
HRESULT CMagicClaw::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	//TargetSearcher
	CCollider_Frustum::FRUSTUM_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fDistance = 8.f;
	tTargetSearcherDesc.fWidth = 3.f;
	tTargetSearcherDesc.fHeight = 1.5f;
	tTargetSearcherDesc.fEndWidth = 6.0f;
	m_pTargetSearcher = static_cast<CCollider_Frustum*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Frustum::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);

	//Bullet
	CBullet::BULLET_DESC tBulletDesc;
	tBulletDesc.eModelProtoLevelID = LEVEL_LOADING;
	tBulletDesc.pShooter = m_pUser;
	strcpy_s(tBulletDesc.strModelProtoName, "he_fi_prop_tgdtable_a02.model");
	m_pBullet = static_cast<CBullet_MagicClaw*>( m_pGameInstance->Clone_Proto_Object_Stock(CBullet_MagicClaw::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(true);
	return S_OK;
}

void CMagicClaw::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	m_pBullet->Update(fTimeDelta); 
}
void CMagicClaw::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pBullet->Late_Update(fTimeDelta);
}
void CMagicClaw::AnimEventFunc1()
{
	//SearchTarget :FustumCollider 켜기
	m_pTargetSearcher->Update(XMMatrixTranslation(0, m_pUser->Get_BodyCollisionOffset().y, -1) * m_pUser->Get_Transform()->Get_WorldMatrix());
	CCharacter* pTarget =  SearchTarget();
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	if(pTarget)
	{
		_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
		fDmg = m_pUser->Get_Stat().iATK * fDmg*0.01;
		m_pBullet->Invoke(fDmg, pTarget);
	}
}
CCharacter* CMagicClaw::SearchTarget()
{
	auto listMonster= m_pGameInstance->Get_GameObjectList(LAYERID::LAYER_MONSTER);
	_float fMinDistance = FLT_MAX;
	CCharacter* pReturn = nullptr;
	for (auto& pMonster : *listMonster)
	{
		CCharacter* pTmpCharacter = static_cast<CCharacter*>(pMonster);
		if (false == pTmpCharacter->Is_Targetable())
			continue;
		if (Check_InRange(pTmpCharacter))
		{
			_float fTmpDistance = XMVectorGetX(XMVector3Length(pTmpCharacter->Get_Position() - m_pUser->Get_Position()));
			if (fTmpDistance < fMinDistance)
			{
				fMinDistance = fTmpDistance;
				pReturn = pTmpCharacter;
			}
		}
	}
	return pReturn;
}
_bool CMagicClaw::Check_InRange(CCharacter* pOther)
{
	if (pOther->Get_Team() == m_pUser->Get_Team())
		return false;

	auto p =pOther->Get_Collider(0);
	return m_pTargetSearcher->Intersects((CColliderBase*)p);

}


HRESULT CMagicClaw::Render_Collider()
{
	return m_pTargetSearcher->Render();
}


CMagicClaw* CMagicClaw::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CMagicClaw* pInstance = new CMagicClaw;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CMagicClaw Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMagicClaw::Free()
{
	__super::Free();
	Safe_Release(m_pTargetSearcher);
	Safe_Release(m_pBullet);
}

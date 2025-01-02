#include "stdafx.h"
#include "Skill.h"
#include "Character.h"
#include "Animation.h"
#include "UIBundle.h"
#include "ModelObject.h" 
#include "BasicAttack.h"
#include "Teleport.h"
#include "MagicClaw.h"
#include "GameInstance.h"
#include "FlameWave.h"
#include "Kindling.h"
#include "FireTornado.h"
#include "BBQParty.h"
#include "FakeMeteor.h"
#include "WildFire.h"
#include "SnailAttack.h"
#include "CrabAttack.h"
#include "StumpKick.h"
#include "BayarAttackA.h"
#include "BayarAttackB.h"
#include "BayarAttackC.h"
#include "BayarAttackD.h"
#include "BayarSkyJump.h"
#include "BayarKnockOff.h"
#include "BayarStoneSpike.h"
#include "BayarPalmStrike.h"
#include "Collider.h"
#include "Player.h"

CSkill::CSkill()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CSkill::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	m_pSkillDesc = pSkillData;

	m_pUser = pUser;
	m_fCoolTimeAcc = m_pSkillDesc->fCoolTime;
    Initialzie_AnimEvent();
	m_pUIBundle = UIBUNDLE;
	return S_OK;
}

void CSkill::Update_CastingTime(_float fDeltaTime)
{
	if (false == Is_CastingType())
		return;
	if (m_bCastingComplete)
		return;
	m_fCastingRatio = m_pUser->Get_AnimationProgress(m_pSkillDesc->vecAnimation[0]);
	m_pUIBundle->Update_CastingRatio(m_fCastingRatio);
	if (m_fCastingRatio >= 1)
	{
		m_bCastingComplete = true;
		On_CastingEnd();
		m_pUser->On_CastingEnd(this);
		m_fCastingRatio = 0.f;
		m_fCoolTimeAcc = 0.f;
	}
}


void CSkill::Use()
{
	if (false == Is_Available())
		return;
	if (m_pUser->Use_Skill(this))
	{
		Consume_Cost();
		if (Is_CastingType())
		{
			m_bCastingComplete = false;
			UIBUNDLE->Update_CastingRatio(0);
		}
		else
			m_fCoolTimeAcc = 0;
		On_SkillUsed();

	}
}

void CSkill::Cancel_Casting()
{
	m_bCastingComplete = true;
	m_fCastingRatio = 0.f;
	m_pUser->On_CastingEnd(this);
}

void CSkill::Register_AnimEvent(CModelObject* pObj)
{
	if (m_mapAnimEvent.empty())
		return;
	for (auto& pairAnim : m_mapAnimEvent)
	{
		for (auto& pairAnimEvent : pairAnim.second)
		{
			pObj->Register_AnimEvent(pairAnim.first, pairAnimEvent);
		}
	}

}

_int CSkill::Get_NextAnimation(_uint iAnimIdx)
{
	vector <_uint> ::iterator iter = m_pSkillDesc->vecAnimation.begin();
	for (; iter != m_pSkillDesc->vecAnimation.end(); ++iter)
	{
		if ((*iter) == iAnimIdx)
		{
			++iter;
			if (iter == m_pSkillDesc->vecAnimation.end())
				return -1;
			return *iter;
		}
	}
	return -1;
}

vector<_uint>& CSkill::Get_AnimIdcies()
{
	return m_pSkillDesc->vecAnimation;
}


_bool CSkill::Is_CoolReady()
{
	return m_fCoolTimeAcc >= m_pSkillDesc->fCoolTime;
}

_bool CSkill::Is_EnoughCost()
{
	SKILL_COST_TYPE eCostType = m_pSkillDesc->eCostType;
	switch (eCostType)
	{
	case Client::SKILL_COST_TYPE::NONE:
		return true;
	case Client::SKILL_COST_TYPE::SP:
		return m_pSkillDesc->iCost <= m_pUser->Get_Stat().iSP;
	case Client::SKILL_COST_TYPE::EP:
		return m_pSkillDesc->iCost <= m_pUser->Get_Stat().iEP;
	case Client::SKILL_COST_TYPE::LAST:
	default:
		break;
	}
	return false;
}

_bool CSkill::Is_CastingType()
{
	return m_pSkillDesc->eCastingType == SKILL_CASTING_TYPE::CASTING;
}

_bool CSkill::Is_CastingComplete()
{
	if (m_pSkillDesc->eCastingType != SKILL_CASTING_TYPE::CASTING)
		return true;
	if (m_pUser->Get_CurrentAnimIdx() != m_pSkillDesc->vecAnimation[0])
		return true;
	return m_bCastingComplete;
}

_bool CSkill::Is_Available()
{
	if (false == Is_CoolReady())
		return false;
	if (false == Is_EnoughCost())
		return false;
	if (m_bNeedWeapon)
		return static_cast<CPlayer*>(m_pUser)->Is_Weapon();
	return true;
}

void CSkill::Initialzie_AnimEvent()
{
	for (auto& eventTime : m_pSkillDesc->mapAnimEventTime)
	{
 		_uint iAnimIdx = m_pSkillDesc->vecAnimation[eventTime.first];
		ANIM_EVENT tAnimEvent;
		for (auto& pairAnimEvent : eventTime.second)
		{
			tAnimEvent.fTime = pairAnimEvent.first;
			SKILL_MOTION_EVENT eEvent = pairAnimEvent.second;
			switch (eEvent)
			{
			case Client::SKILL_MOTION_EVENT::CAST:
				tAnimEvent.pFunc = std::bind(&CSkill::On_SkillUsed, this);
				break;
			case Client::SKILL_MOTION_EVENT::FIRE:
				tAnimEvent.pFunc = std::bind(&CSkill::Fire, this);
				break;
			case Client::SKILL_MOTION_EVENT::ATTACK_END:
				tAnimEvent.pFunc = std::bind(&CSkill::On_AttackEnd, this);
				break;
			case Client::SKILL_MOTION_EVENT::LAST:
				break;
			default:
				break;
			}
			m_mapAnimEvent[iAnimIdx].push_back( tAnimEvent );
		}
	}
}

void CSkill::Fire()
{
	int a = 0;
}

void CSkill::On_AttackEnd()
{
}

void CSkill::Consume_Cost()
{
	SKILL_COST_TYPE eCostType = m_pSkillDesc->eCostType;
	Stat* pUserStat = m_pUser->Get_Stat_Ref();
	Stat* pUserDefaultStat = m_pUser->Get_DefaultStat_Ref();
	switch (eCostType)
	{
	case Client::SKILL_COST_TYPE::NONE:
		return;
	case Client::SKILL_COST_TYPE::SP:
		pUserStat->iSP = clamp(pUserStat->iSP - (_int)m_pSkillDesc->iCost, 0, pUserDefaultStat->iSP);
		break;
	case Client::SKILL_COST_TYPE::EP:
		pUserStat->iEP = clamp(pUserStat->iEP - (_int)m_pSkillDesc->iCost, 0, pUserDefaultStat->iEP);
		break;
	case Client::SKILL_COST_TYPE::LAST:
	default:
		break;
	}
}

void CSkill::SearchTarget(list<CGameObject*>* pOutList, LAYERID eLayerID)
{
	auto listObj = m_pGameInstance->Get_GameObjectList(eLayerID);
	if (nullptr == listObj) return;
	for (auto& pObj : *listObj)
	{
		CCharacter* pTmpCharacter = static_cast<CCharacter*>(pObj);
		if (false == pTmpCharacter->Is_Targetable())
			continue;
		if (Check_Collision(pTmpCharacter))
			pOutList->push_back(pTmpCharacter);
	}
}

CCharacter* CSkill::SearchTarget(LAYERID eLayerID)
{
	auto listMonster = m_pGameInstance->Get_GameObjectList(eLayerID);
	if (nullptr == listMonster)return nullptr;
	_float fMinDistance = FLT_MAX;
	CCharacter* pReturn = nullptr;
	for (auto& pMonster : *listMonster)
	{
		CCharacter* pTmpCharacter = static_cast<CCharacter*>(pMonster);
		if (false == pTmpCharacter->Is_Targetable())
			continue;
		if (pTmpCharacter->Get_Team() == m_pUser->Get_Team())
			continue;
		if (Check_Collision(pTmpCharacter))
		{
			_float fTmpDistance = XMVectorGetX(XMVector3Length(pTmpCharacter->Get_TransformPosition() - m_pUser->Get_TransformPosition()));
			if (fTmpDistance < fMinDistance)
			{
				fMinDistance = fTmpDistance;
				pReturn = pTmpCharacter;
			}
		}
	}
	return pReturn;
}

_bool CSkill::Check_Collision(CGameObject* pOther)
{
	CCharacter* pCharacter = dynamic_cast<CCharacter*>(pOther);
	if (nullptr == pCharacter)
		return false;
	if (false == pCharacter->Is_Valid())
		return false;
	if (pCharacter->Get_Team() == m_pUser->Get_Team())
		return false;

	return m_pTargetSearcher->Intersects(pCharacter->Get_Collider(0));

}

_float CSkill::Calc_Damg(_bool& bOutCritical)
{
	_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[(_uint)SKILL_DATA_ID::DAMG] + m_pSkillDesc->vecData[(_uint)SKILL_DATA_ID::DAMG];
	fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;
	bOutCritical = m_pUser->Judge_Critical();
	fDmg *= bOutCritical ? 1.5 : 1.f;
	return fDmg;
}


CSkill* CSkill::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CSkill* pInstance = nullptr;
	SKILL_ID eID = pSkillData->eID;
	switch (eID)
	{
	case Client::SKILL_ID::BASIC_ATTACK:
		pInstance = CBasicAttack::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::JUMP_BASIC_ATTACK:
		pInstance = CBasicAttack::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::TELEPORT:
		pInstance = CTeleport::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::MAGIC_CLAW:
		pInstance = CMagicClaw::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::FLAME_WAVE:
		pInstance = CFlameWave::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::FIRE_TORNADO:
		pInstance = CFireTornado::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::KINDLING:
		pInstance = CKindling::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::KINDLING2:

		break;
	case Client::SKILL_ID::BBQ_PARTY:
		pInstance = CBBQParty::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::FAKE_METEOR:
		pInstance = CFakeMeteor::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::WILD_FIRE:
		pInstance = CWildFire::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::WILD_FIRE2:

		break;
	case Client::SKILL_ID::SNAIL_ATTACK:
		pInstance = CSnailAttack::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::CRAB_ATTACK:
		pInstance = CCrabAttack::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::STUMP_CHARGE_ATTACK:

		break;
	case Client::SKILL_ID::STUMP_KICK:
		pInstance = CStumpKick::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::STUMP_BUNTING:

		break;
	case Client::SKILL_ID::BAYAR_ATTACK_A:
		pInstance = CBayarAttackA::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::BAYAR_ATTACK_B:
		pInstance = CBayarAttackB::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::BAYAR_ATTACK_C:
		pInstance = CBayarAttackC::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::BAYAR_ATTACK_D:
		pInstance = CBayarAttackD::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::BAYAR_SKY_JUMP:
		pInstance = CBayarSkyJump::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::BAYAR_KNOCK_OFF:
		pInstance = CBayarKnockOff::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::BAYAR_STONE_SPIKE:
		pInstance = CBayarStoneSpike::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::BAYAR_PALM_STRIKE:
		pInstance = CBayarPalmStrike::Create(pSkillData, pUser);

		break;
	case Client::SKILL_ID::LAST:
		break;
	default:
		break;
	}
	if (pInstance == nullptr)
		pInstance = CBasicAttack::Create(pSkillData, pUser);
	return pInstance;

}

void CSkill::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTargetSearcher);
}

_int CSkill::Get_StackCount_Quick()
{
	return -1;
}

_float CSkill::Get_CoolTimeRatio()
{
	return m_fCoolTimeAcc/ m_pSkillDesc->fCoolTime;
}

const _char* CSkill::Get_IconTag()
{
	return m_pSkillDesc->strIconImageTag;
}



SkillData::SkillData(const json& jData)
{
	SKILL_ID iID = jData["Id"];

	eID = iID;
	eCastingType = jData["CastType"];
	string str = jData["Name"];
	std::copy(str.begin(), str.end(), szName);
	str = jData["Desc"];
	std::copy(str.begin(), str.end(), szDesc);
	iLevel = jData["Level"];
	iMaxLevel = jData["MaxLevel"];
	eCostType = jData["CostType"];
	iCost = jData["Cost"];
	fCoolTime = jData["CoolTime"];
	str = jData["IconImg"];
	std::copy(str.begin(), str.end(),strIconImageTag);
	fRange = jData["Range"];
	for (auto& data : jData["Data"])
		vecData.push_back(data);
	for (auto& data : jData["LevelUpData"])
		vecLevelUpData.push_back(data);
	for (auto& data : jData["Preceding"])
	{
		SKILL_ID eID = data[0];
		_uint iLev = data[1];
		mapPreceding.insert({ eID , iLev });
	}
	for (auto& data : jData["Animation"])
		vecAnimation.push_back(data);
	for (auto& data : jData["EventTime"])
	{
		_uint iAnimIdx = data[0];
		_float fTime = data[1];
		SKILL_MOTION_EVENT eEvent = data[2];
		mapAnimEventTime[iAnimIdx].push_back({ fTime,eEvent });
	}
}

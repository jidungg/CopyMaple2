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

CSkill::CSkill()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CSkill::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	m_pSkillDesc = pSkillData;

	m_pUser = pUser;
	m_fCoolTimeAcc = pSkillData->fCoolTime;
	Initialzie_AnimEvent();
	return S_OK;
}
void CSkill::Update_CoolTime(_float fDeltaTime)
{

	m_fCoolTimeAcc += fDeltaTime;
}

void CSkill::Update_CastingTime(_float fDeltaTime)
{
	if (m_bCastingComplete)
		return;
	m_fCastingRatio = m_pUser->Get_AnimationProgress(m_pSkillDesc->vecAnimation[0]);
	UIBUNDLE->Update_CastingRatio(m_fCastingRatio);
	if (m_fCastingRatio >= 1)
	{
		m_bCastingComplete = true;
		m_pUser->On_CastingEnd(this);
		m_fCoolTimeAcc = 0.f;
	}
}

void CSkill::Update(_float fTimeDelta)
{
}

void CSkill::Late_Update(_float fTimeDelta)
{
}

void CSkill::Use()
{
	if (false == Is_Available())
		return;
	if (m_pUser->Use_Skill(this))
	{
		m_bCastingComplete = false;
		UIBUNDLE->Update_CastingRatio(0);
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
	for (auto& animEvnet : m_mapAnimEvent)
	{
		pObj->Register_AnimEvent(animEvnet.first, animEvnet.second);
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
	return true;
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
	case Client::SKILL_ID::TELEPORT:
		pInstance = CTeleport::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::MAGIC_CLAW:
		pInstance = CMagicClaw::Create(pSkillData, pUser);
		break;
	case Client::SKILL_ID::FLAME_WAVE:

		break;
	case Client::SKILL_ID::FIRE_TORNADO:

		break;
	case Client::SKILL_ID::KINDLING:

		break;
	case Client::SKILL_ID::KINDLING2:

		break;
	case Client::SKILL_ID::BBQ_PARTY:

		break;
	case Client::SKILL_ID::FAKE_METEOR:

		break;
	case Client::SKILL_ID::WILD_FIRE:

		break;
	case Client::SKILL_ID::WILD_FIRE2:

		break;
	case Client::SKILL_ID::SNAIL_ATTACK:
		
		break;
	case Client::SKILL_ID::CRAB_ATTACK:

		break;
	case Client::SKILL_ID::STUMP_CHARGE_ATTACK:

		break;
	case Client::SKILL_ID::STUMP_KICK:

		break;
	case Client::SKILL_ID::STUMP_BUNTING:

		break;
	case Client::SKILL_ID::BAYAR_ATTACK_A:

		break;
	case Client::SKILL_ID::BAYAR_ATTACK_B:

		break;
	case Client::SKILL_ID::BAYAR_ATTACK_C:

		break;
	case Client::SKILL_ID::BAYAR_ATTACK_D:

		break;
	case Client::SKILL_ID::BAYAR_ATTACK_E:

		break;
	case Client::SKILL_ID::BAYAR_ATTACK_F:
		break;
	case Client::SKILL_ID::BAYAR_ATTACK_G:

		break;
	case Client::SKILL_ID::BAYAR_ATTACK_H:

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
}

const _char* CSkill::Get_IconTag()
{
	return m_pSkillDesc->strIconImageTag;
}

_float CSkill::Get_CoolTimeRatio()
{
	return m_fCoolTimeAcc / m_pSkillDesc->fCoolTime;
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
		mapAnimEventTime.insert({ iAnimIdx, fTime });
	}
}

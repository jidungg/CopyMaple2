#include "stdafx.h"
#include "SkillManager.h"

IMPLEMENT_SINGLETON(CSkillManager)
CSkillManager::CSkillManager()
{

}

void CSkillManager::Insert_Data(json& jSkillData)
{
	SKILL_ID iID = jSkillData["id"];
	SKILL_DESC& desc =  m_SkillData[(_uint)iID];

	desc.eID = iID;
	desc.eType = jSkillData["type"];
	string str = jSkillData["name"];
	std::copy(str.begin(), str.end(), desc.szName);
	str = jSkillData["desc"];
	std::copy(str.begin(), str.end(), desc.szDesc);
	desc.iLevel = jSkillData["level"];
	desc.iMaxLevel = jSkillData["maxlevel"];
	desc.eCostType = jSkillData["costtype"];
	desc.iCost = jSkillData["cost"];
	desc.fCoolTime = jSkillData["cooltime"];
	str = jSkillData["iconimg"];
	std::copy(str.begin(), str.end(), desc.strIconImageTag);
	for (auto& data : jSkillData["data"])
	{
		desc.vecData.push_back(data);
	}
	for (auto& data : jSkillData["levelupdata"])
	{
		desc.vecLevelUpData.push_back(data);
	}
	for (auto& data : jSkillData["preceding"])
	{
		SKILL_ID eID = data[0];
		_uint iLev = data[1];
		desc.mapPreceding.insert({ eID , iLev });
	}

	
}

void CSkillManager::Free()
{
	__super::Free();
}

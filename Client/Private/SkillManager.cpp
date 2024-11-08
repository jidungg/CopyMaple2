#include "stdafx.h"
#include "SkillManager.h"

IMPLEMENT_SINGLETON(CSkillDataBase)
CSkillDataBase::CSkillDataBase()
{

}

void CSkillDataBase::Insert_Data(json& jSkillData)
{
	SKILL_ID iID = jSkillData["id"];
	SKILL_DATA& desc =  m_SkillData[(_uint)iID];

	desc.eID = iID;
	desc.eCastingType = jSkillData["type"];
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
	for (auto& data : jSkillData["animation"])
	{
		desc.vecAnimation.push_back(data);
	}
}

void CSkillDataBase::Free()
{
	__super::Free();
}

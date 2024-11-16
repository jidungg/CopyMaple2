#include "stdafx.h"
#include "SkillManager.h"

IMPLEMENT_SINGLETON(CSkillDataBase)
CSkillDataBase::CSkillDataBase()
{

}

void CSkillDataBase::Insert_Data(json& jSkillData)
{
	SKILL_ID iID = jSkillData["Id"];
	m_SkillData[(_uint)iID] = SkillData(jSkillData);
}

HRESULT CSkillDataBase::Load_Data()
{
	json j;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/SkillData.json", &j)))
		return E_FAIL;
	for (auto& skill : j["skills"])
	{
		Insert_Data(skill);
	}
	return S_OK;
}

void CSkillDataBase::Free()
{
	__super::Free();
}

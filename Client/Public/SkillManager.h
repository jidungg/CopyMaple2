#pragma once
#include "Base.h"
#include "Skill.h"

BEGIN(Client)

class CSkillManager :
	public CBase
{
	DECLARE_SINGLETON(CSkillManager)
private:
	CSkillManager();
	virtual ~CSkillManager() = default;

public:
	void Insert_Data(json& jSkillData);

	SKILL_DESC* Get_SkillData(SKILL_ID eSkillId) { return &m_SkillData[(_uint)eSkillId]; }

private:
	SKILL_DESC m_SkillData[(_uint)SKILL_ID::LAST];
public:
	void Free();
};
#define SKILLMAN CSkillManager::GetInstance()
END
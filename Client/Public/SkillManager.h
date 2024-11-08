#pragma once
#include "Base.h"
#include "Skill.h"

BEGIN(Client)

class CSkillDataBase :
	public CBase
{
	DECLARE_SINGLETON(CSkillDataBase)
private:
	CSkillDataBase();
	virtual ~CSkillDataBase() = default;

public:
	void Insert_Data(json& jSkillData);

	SKILL_DATA* Get_SkillData(SKILL_ID eSkillId) { return &m_SkillData[(_uint)eSkillId]; }

private:
	SKILL_DATA m_SkillData[(_uint)SKILL_ID::LAST];
public:
	void Free();
};
#define SKILLDB CSkillDataBase::GetInstance()
END
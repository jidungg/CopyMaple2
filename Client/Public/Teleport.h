#pragma once
#include "Skill.h"


BEGIN(Client)
class CTeleport :
    public CSkill
{
private:
	CTeleport();
	virtual ~CTeleport() = default;

public:
	void AnimEventFunc1();
protected:
	virtual void Initialzie_AnimEvent();
private:

public:
	static CTeleport* Create(SKILL_DATA* pSkillData, CCharacter* pUser);


};

END
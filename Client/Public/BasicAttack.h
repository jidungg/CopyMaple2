#pragma once
#include "Skill.h"

BEGIN(Client)
class CBasicAttack :
    public CSkill
{
private:
	CBasicAttack();
	virtual ~CBasicAttack() = default;

public:
	void AnimEventFunc1();
protected:
    virtual void Initialzie_AnimEvent() override;

public:
	static CBasicAttack* Create(SKILL_DATA* pSkillData, CCharacter* pUser);

};

END
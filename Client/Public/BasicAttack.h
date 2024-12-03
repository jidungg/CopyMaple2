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

protected:

	virtual void On_Cast() override;
	virtual void On_CastingEnd() override;
	virtual void Fire() override;
public:
	static CBasicAttack* Create(SKILL_DATA* pSkillData, CCharacter* pUser);


};

END
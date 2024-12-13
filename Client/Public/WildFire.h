#pragma once
#include "Skill.h"

BEGIN(Client)
class CBullet_WildFire;
class CEffModelObject;

class CWildFire :
    public CSkill
{
private:
	CWildFire();
	virtual ~CWildFire() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	void On_SkillUsed() override;
	void On_CastingEnd() override;
	void Fire() override;

private:
private:
	CBullet_WildFire* m_pBullet = { nullptr };
	CEffModelObject* m_pCastEffect1 = { nullptr };

public:
	static CWildFire* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
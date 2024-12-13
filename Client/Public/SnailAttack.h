#pragma once
#include "Skill.h"
#include "set"

BEGIN(Engine)
class CCollider_Sphere;
class CGameObject;
END

BEGIN(Client)
class CBullet;
class CEffModelObject;
class CSnailAttack :
    public CSkill
{
private:
	CSnailAttack();
	virtual ~CSnailAttack() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	void On_SkillUsed() override;
	void On_CastingEnd() override;
	void Fire() override;


private:

	CEffModelObject* m_pCastEffect = { nullptr };
	set<CGameObject*> m_setHitObject;
public:
	static CSnailAttack* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
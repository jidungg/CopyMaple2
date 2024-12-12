#pragma once
#include "Skill.h"

BEGIN(Engine)
class CCollider_Sphere;
class CGameObject;
END


BEGIN(Client)
class CBullet;
class CEffModelObject;
class CBayarAttackA :
    public CSkill
{
private:
	CBayarAttackA();
	virtual ~CBayarAttackA() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	virtual void On_Cast() override;
	virtual void On_CastingEnd() override;
	virtual void Fire() override;
	virtual void On_AttackEnd() override;

private:

	CEffModelObject* m_pCastEffect = { nullptr };
public:
	static CBayarAttackA* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
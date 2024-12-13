#pragma once
#include "Skill.h"
BEGIN(Engine)
class CCollider_Sphere;
class CGameObject;
END

BEGIN(Client)
class CBullet_BayarAttackD;
class CEffModelObject;
class CBayarAttackD :
    public CSkill
{
private:
	CBayarAttackD();
	virtual ~CBayarAttackD() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	virtual void On_SkillUsed() override;
	virtual void On_CastingEnd() override;
	virtual void Fire() override;
	virtual void On_AttackEnd() override;


private:
	CBullet_BayarAttackD* m_pBullet = { nullptr };
	CEffModelObject* m_pAttackEffect = { nullptr };
	CEffModelObject* m_pChargeEffect = { nullptr };
	_float m_fMoveSpeed = { 8.f };
public:
	static CBayarAttackD* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;

};

END
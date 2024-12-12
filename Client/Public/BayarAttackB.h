#pragma once
#include "Skill.h"

BEGIN(Engine)
class CCollider_Sphere;
class CGameObject;
END

BEGIN(Client)
class CBullet_BayarAttackB;
class CEffModelObject;
class CBayarAttackB :
    public CSkill
{
protected:
	explicit CBayarAttackB();
	virtual ~CBayarAttackB() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	virtual void On_Cast() override;
	virtual void On_CastingEnd() override;
	virtual void Fire() override;
	virtual void On_AttackEnd() override;

	void Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
private:
	CBullet_BayarAttackB* m_pBullet = { nullptr };
	CEffModelObject* m_pCastEffect = { nullptr };
	_float m_fMoveSpeed = { 0.7f };
public:
	static CBayarAttackB* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;

};

END
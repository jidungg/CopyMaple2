#pragma once
#include "Skill.h"

BEGIN(Client)
class CEffModelObject;
class CBullet_BayarPalmStrike;
class CBayarPalmStrike :
    public CSkill
{
private:
	CBayarPalmStrike();
	virtual ~CBayarPalmStrike() = default;

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
	CEffModelObject* m_pWindEffect = { nullptr };
	CEffModelObject* m_pGroundEffect = { nullptr };

	_vector m_vRGroundEffectOffset = { 0.4f,0.f,1.f,1.f };
	_vector m_vLGroundEffectOffset = { -0.6f,0.f,1.f,1.f };
	_uint m_iAttackCount = 0;
public:
	static CBayarPalmStrike* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
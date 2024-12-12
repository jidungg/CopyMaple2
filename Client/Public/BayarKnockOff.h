#pragma once
#include "Skill.h"

BEGIN(Client)
class CEffModelObject;

class CBayarKnockOff :
    public CSkill
{
private:
	CBayarKnockOff();
	virtual ~CBayarKnockOff() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	virtual void On_Cast() override;
	virtual void On_CastingEnd() override;
	virtual void Fire() override;
	virtual void On_AttackEnd() override;

private:
	CEffModelObject* m_pJumpReadyEffect = { nullptr };
	CEffModelObject* m_pGatheringEffect = { nullptr };

	_float m_fTimeAcc = { 0.f };
	_float m_fDamgInterval = { 0.5f };
public:
	static CBayarKnockOff* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
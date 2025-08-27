#pragma once
#include "BayarAttackB.h"

BEGIN(Client)
class CBayarAttackC :
    public CBayarAttackB
{
private:
	explicit CBayarAttackC();
	virtual ~CBayarAttackC() = default;
public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	virtual void On_SkillUsed() override;
	virtual void On_CastingEnd() override;
	virtual void Fire() override;
	virtual void On_AttackEnd() override;
private:
	CEffModelObject* m_pChargeEffect = { nullptr };

public:
	static CBayarAttackC* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
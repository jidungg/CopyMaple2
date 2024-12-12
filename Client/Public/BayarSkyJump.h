#pragma once
#include "Skill.h"

BEGIN(Client)
class CEffModelObject;
class CBayarSkyJump :
    public CSkill
{
private:
	CBayarSkyJump();
	virtual ~CBayarSkyJump() = default;

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
	CEffModelObject* m_pJumpReadyEffect = { nullptr };
	CEffModelObject* m_pGatheringEffect = { nullptr };
	CEffModelObject* m_pLandEffect = { nullptr };

public:
	static CBayarSkyJump* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
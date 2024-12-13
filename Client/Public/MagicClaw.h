#pragma once
#include "Skill.h"
BEGIN(Engine)
class CCollider_Frustum;
END
BEGIN(Client)
class CEffModelObject;
class CBullet_MagicClaw;
class CMagicClaw :
    public CSkill
{
private:
	CMagicClaw();
	virtual ~CMagicClaw() = default;

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
	CBullet_MagicClaw* m_pBullet = { nullptr };
	CEffModelObject* m_pCastEffect = { nullptr };
public:
	static CMagicClaw* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;

};

END
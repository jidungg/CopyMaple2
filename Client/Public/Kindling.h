#pragma once
#include "Skill.h"
BEGIN(Engine)
class CCollider_Frustum;
END
BEGIN(Client)
class CBullet_Kindling;
class CEffModelObject;
class CKindling :
    public CSkill
{
private:
	CKindling();
	virtual ~CKindling() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:

	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	void On_SkillUsed() override;
	void On_CastingEnd() override;
	void Fire() override;

private:
	CBullet_Kindling* m_pBullet = { nullptr };
	CEffModelObject* m_pCastEffect = { nullptr };
	CEffModelObject* m_pCastEndEffect = { nullptr };
public:
	static CKindling* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;

};

END
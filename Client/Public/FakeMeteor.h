#pragma once
#include "Skill.h"
BEGIN(Engine)
class CCollider_Frustum;
END
BEGIN(Client)
class CBullet_FakeMeteor;
class CEffModelObject;
class CFakeMeteor :
    public CSkill
{
private:
	CFakeMeteor();
	virtual ~CFakeMeteor() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	void On_SkillUsed() override;
	void On_CastingEnd() override;
	void Fire() override;


private:
	CBullet_FakeMeteor* m_pBullet = { nullptr };
	CEffModelObject* m_pCastEffect1 = { nullptr };
	CEffModelObject* m_pCastEffect2 = { nullptr };
public:
	static CFakeMeteor* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
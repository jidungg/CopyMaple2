#pragma once
#include "Skill.h"

BEGIN(Engine)
class CCollider_Frustum;
END
BEGIN(Client)
class CBullet_BBQParty;
class CEffModelObject;
class CBBQParty :
    public CSkill
{
private:
	CBBQParty();
	virtual ~CBBQParty() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:

	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);



	void On_Cast() override;
	void On_CastingEnd() override;
	void Fire() override;

private:

	CCollider_Frustum* m_pTargetSearcher = { nullptr };
	CBullet_BBQParty* m_pBullet = { nullptr };
	CEffModelObject* m_pCastEffect1 = { nullptr };
	CEffModelObject* m_pCastEffect2 = { nullptr };
public:
	static CBBQParty* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
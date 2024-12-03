#pragma once
#include "Skill.h"
BEGIN(Engine)
class CCollider_Sphere;
class CGameObject;
END

BEGIN(Client)
class CBullet;
class CEffModelObject;
class CStumpKick :
    public CSkill
{
private:
	CStumpKick();
	virtual ~CStumpKick() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	void On_Cast() override;
	void On_CastingEnd() override;
	void Fire() override;


private:
	CEffModelObject* m_pCastEffect = { nullptr };
public:
	static CStumpKick* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
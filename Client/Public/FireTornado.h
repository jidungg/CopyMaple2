#pragma once
#include "Skill.h"
BEGIN(Engine)

END
BEGIN(Client)
class CBullet_FireTornado;
class CEffModelObject;
class CFireTornado :
    public CSkill
{
private:
	CFireTornado();
	virtual ~CFireTornado() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:

	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	void On_Cast() override;
	void On_CastingEnd() override;
	void Fire() override;


private:
	CBullet_FireTornado* m_pBullet = { nullptr };
	CEffModelObject* m_pCastEffect = { nullptr };
public:
	static CFireTornado* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
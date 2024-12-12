#pragma once
#include "Skill.h"


BEGIN(Client)
class CEffModelObject;
class CTeleport :
    public CSkill
{
private:
	CTeleport();
	virtual ~CTeleport() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
protected:

	void On_Cast() override;
	void On_CastingEnd() override;
	void Fire() override;
private:
	CEffModelObject* m_pCastEffect = { nullptr };
public:
	static CTeleport* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
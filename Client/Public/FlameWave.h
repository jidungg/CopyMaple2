#pragma once
#include "Skill.h"

BEGIN(Engine)
class CCollider_Frustum;
END

BEGIN(Client)
class CBullet;
class CEffModelObject;
class CFlameWave :
    public CSkill
{

private:
	CFlameWave();
	virtual ~CFlameWave() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;

protected:

	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render_Collider();

	void On_Cast() override;
	void On_CastingEnd() override;
	void Fire() override;

private:
	CEffModelObject* m_pCastEffect = { nullptr };
public:
	static CFlameWave* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
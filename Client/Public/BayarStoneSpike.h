#pragma once
#include "Skill.h"
#include "ObjectPool.h"

BEGIN(Client)
class CBullet_BayarStoneSpike;
class CCubeTerrain;
class CBayarStoneSpike :
    public CSkill
{
private:
	CBayarStoneSpike();
	virtual ~CBayarStoneSpike() = default;

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
	void Launch_Bullets(_vector vTargetPos);
private:
	CObjectPool<CBullet_BayarStoneSpike>* m_pBulletPool = { nullptr };
	_bool m_bAttack = { false };
	_float m_fAttackInterval = { 0.75f };
	_float m_fTimeAcc = { 0.f };

	CCubeTerrain* m_pTerrain = { nullptr };
public:
	static CBayarStoneSpike* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
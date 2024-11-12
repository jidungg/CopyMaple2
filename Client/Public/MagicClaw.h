#pragma once
#include "Skill.h"
BEGIN(Engine)
class CCollider_Frustum;
END
BEGIN(Client)
class CBullet;
class CMagicClaw :
    public CSkill
{
	enum class SKILL_DATA_ID
	{
		DAMG,
		LAST
	};

private:
	CMagicClaw();
	virtual ~CMagicClaw() = default;

public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser) override;
	void AnimEventFunc1();
protected:
	virtual void Initialzie_AnimEvent()override;
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render_Collider();

private:
	CCharacter* SearchTarget();
	_bool Check_InRange(CCharacter* pOther);
private:
	CCollider_Frustum* m_pTargetSearcher = { nullptr };
	CBullet* m_pBullet = { nullptr };

public:
	static CMagicClaw* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;
};

END
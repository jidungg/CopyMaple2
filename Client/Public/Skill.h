#pragma once
#include "Base.h"
#include "SlotItem.h"

BEGIN(Client)
typedef struct SkillDesc
{
	SkillDesc() = default;
	SkillDesc(json& jSkillData);
	SKILL_ID eID;
	SKILL_TYPE eCastingType;
	_char szName[MAX_PATH] = ("");
	_char szDesc[MAX_PATH] = ("");
	_uint iLevel = { 0 };
	_uint iMaxLevel = { 0 };
	SKILL_COST_TYPE eCostType;
	_uint iCost = { 0 };
	_float fCoolTime = { 0.f };
	_char strIconImageTag[MAX_PATH] = ("");
	vector<float> vecData;
	vector<float> vecLevelUpData;
	map<SKILL_ID, _uint> mapPreceding;
}SKILL_DESC;
class CCharacter;
class CSkill :
	public CBase, public IQuickItem
{
private:
	CSkill();
	virtual ~CSkill() = default;
public:
	HRESULT Initialize(SKILL_DESC* pSkillData, CCharacter* pUser);
	void Update(_float fDeltaTime );

	void Use();

	SKILL_DESC* Get_SkillDesc() { return m_pSkillDesc; }
private:

	SKILL_DESC* m_pSkillDesc = { nullptr };
	CCharacter* m_pUser = {nullptr};
	_float m_fCurrentCasting = { 0.f };
	_float m_fCoolTimeAcc = { 0.f };
public:
	static CSkill* Create(SKILL_DESC* pSkillData, CCharacter* pUser);
	virtual void Free() override;

	// IQuickItem을(를) 통해 상속됨
	const _char* Get_IconTag() override;
	_float Get_CoolTimeRatio() override;
};

END
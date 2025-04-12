#pragma once
#include "Base.h"
#include "SlotItem.h"
BEGIN(Engine)
class CAnimation;
class CGameInstance;
class CColliderBase;
class CGameObject;
END
BEGIN(Client)

typedef struct SkillData
{
	SkillData() = default;
	SkillData(const json& jData);
	SKILL_ID eID;
	SKILL_CASTING_TYPE eCastingType;

	SKILL_TARGET_TYPE eTargetType;
	SKILL_RANGE_TYPE eRangeType;
	//.콜라이더 크기, 오픠ㅏ셋 정보 추가
	_tchar szName[MAX_PATH] = TEXT("");
	_tchar szDesc[MAX_PATH] = TEXT("");
	_uint iLevel = { 0 };
	_uint iMaxLevel = { 0 };
	SKILL_COST_TYPE eCostType;
	_uint iCost = { 0 };
	_float fCoolTime = { 0.f };
	_tchar strIconImageTag[MAX_PATH] = TEXT("");
	_float fRange = { 1.f };
	vector<float> vecData;
	vector<float> vecLevelUpData;
	map<SKILL_ID, _uint> mapPreceding;
	vector <_uint> vecAnimation;
	map<_uint, list<pair<_float, SKILL_MOTION_EVENT>>> mapAnimEventTime;
}SKILL_DATA;
//use가 호출되면 vecAnimation 에 담긴 첫 번째 애니메이션을 Player에게 전달한다.
// Player는 이 애니메이션을 실행하고 애니메이션 종료 시 마다 Skill에게 알림을 줌.
// Skill은 알림받은 애니메이션의 다음 애니메이션을 반환한다.
// 다음 애니메이션이 없으면 -1을 반환한다.
// Player는 -1을 받으면 스킬 종료
//아니면 다음 애니메이션 실행.
class CCharacter;
class CModelObject;

class CSkill :
	public CBase, public IQuickItem
{
public:
	enum class SKILL_DATA_ID
	{
		DAMG,
		LAST
	};
protected:
	CSkill();
	virtual ~CSkill() = default;
public:
	virtual HRESULT Initialize(SKILL_DATA* pSkillData, CCharacter* pUser);
	//void Update_CoolTime(_float fDeltaTime );
	void Update_CastingTime(_float fDeltaTime );
	virtual void Update(_float fTimeDelta) {};
	virtual void Late_Update(_float fTimeDelta) {};
	virtual HRESULT Render_Collider() { return S_OK; }
	void Use();
	void Cancel_Casting();
	void Register_AnimEvent(CModelObject* pObj);


	virtual const _tchar* Get_IconTag() override;
	//virtual _float Get_CoolTimeRatio() override;

	SKILL_DATA* Get_SkillDesc() { return m_pSkillDesc; }
	_int Get_NextAnimation(_uint iAnimIdx);
	vector<_uint>& Get_AnimIdcies();
	_bool Is_CastingComplete();
	_bool Is_CoolReady();
	_bool Is_EnoughCost();
	_bool Is_CastingType();
	virtual _bool Is_Available();
	_float Calc_Damg(_bool& bOutCritical);

protected:
	virtual void Initialzie_AnimEvent() ;
	virtual void On_SkillUsed() abstract;
	virtual void On_CastingEnd() abstract;
	virtual void Fire();
	virtual void On_AttackEnd();
	virtual _float Get_CoolTimeRatio() override;
	virtual _int Get_StackCount_Quick() override;

	void Consume_Cost();
	void SearchTarget(list<CGameObject*>* pOutList, LAYERID eLayerID);
	CCharacter* SearchTarget(LAYERID eLayerID);
	virtual _bool Check_Collision(CGameObject* pOther);
protected:
	CGameInstance* m_pGameInstance = { nullptr };
	SKILL_DATA* m_pSkillDesc = { nullptr };
	CCharacter* m_pUser = {nullptr};
	_float m_fCastingRatio = { 0.f };
	//_float m_fCoolTimeAcc = { 0.f };
	_bool m_bCastingComplete = { false };
	map<_uint, list< ANIM_EVENT>> m_mapAnimEvent;
	CColliderBase* m_pTargetSearcher = { nullptr };
	_bool m_bNeedWeapon = { false };
	class CUIBundle* m_pUIBundle = { nullptr };
public:
	static CSkill* Create(SKILL_DATA* pSkillData, CCharacter* pUser);
	virtual void Free() override;

};

END
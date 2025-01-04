#pragma once
#include "Character.h"
#include "Status.h"

BEGIN(Client)
class CWayFinder;
class CCubeTerrain;
class CWorldUIHPBar;
typedef struct MonsterData
{
	MonsterData() = default;
	MonsterData(json& js)
	{
		eMonID = js["Id"];
		eMonGrade = js["Grade"];
		string str = js["Name"];
		std::copy(str.begin(), str.end(), strMonsterName);
		str = js["Desc"];
		std::copy(str.begin(), str.end(), strMonsterDesc);
		str = js["Model"];
		std::copy(str.begin(), str.end(), strModelTag);
		tStat = STATUS(js["Status"]);
		vecSkillID = js["Skill"].get<vector<SKILL_ID>>();
		fBodyCollisionRadius = js["BodyCollisionRadius"];
		json jOffset = js["BodyCollisionOffset"];
		fBodyCollisionOffset = { jOffset[0],jOffset[1],jOffset[2] };

		fDetectionRange = js["DetectionRange"];
		fChaseRange = js["ChaseRange"];
		auto jScale = js["Scale"];
		vScale = { jScale[0].get<_float>(),jScale[1].get<_float>(),jScale[2].get<_float>() };
		json& jmapAnimIdx = js["AnimIdx"];
		mapAnimIdx[M_AS_ATTACK_IDLE] = jmapAnimIdx["ATTACK_IDLE"].get<vector<_uint>>();
		mapAnimIdx[M_AS_BORE] = jmapAnimIdx["BORE"].get<vector<_uint>>();
		mapAnimIdx[M_AS_DAMG] = jmapAnimIdx["DAMG"].get<vector<_uint>>();
		mapAnimIdx[M_AS_DEAD] = jmapAnimIdx["DEAD"].get<vector<_uint>>();
		mapAnimIdx[M_AS_IDLE] = jmapAnimIdx["IDLE"].get<vector<_uint>>();
		mapAnimIdx[M_AS_REGEN] = jmapAnimIdx["REGEN"].get<vector<_uint>>();
		mapAnimIdx[M_AS_RUN] = jmapAnimIdx["RUN"].get<vector<_uint>>();
		mapAnimIdx[M_AS_STUN] = jmapAnimIdx["STUN"].get<vector<_uint>>();
		mapAnimIdx[M_AS_WALK] = jmapAnimIdx["WALK"].get<vector<_uint>>();
	}
	MONSTER_ID eMonID = { MONSTER_ID::LAST };
	MONSTER_GRADE  eMonGrade = { MONSTER_GRADE::LAST };
	_char strMonsterName[MAX_PATH] = ("");
	_char strMonsterDesc[MAX_PATH] = ("");
	_char strModelTag[MAX_PATH] = ("");
	_float fDetectionRange{ 3.f };
	_float fChaseRange{ 4.f };
	_float fBodyCollisionRadius;
	_float3 fBodyCollisionOffset;
	unordered_map<MON_STATE, vector<_uint>> mapAnimIdx;
	_float3 vScale = { 1,1,1 };
	STATUS tStat;
	vector<SKILL_ID> vecSkillID;

}MONSTER_DATA;


class CMonster :
	public CCharacter
{
public:
	enum COLLIDER_ID
	{
		COLLIDER_BODY,
		COLLIDER_DETECT,
		COLLIDER_LAST
	};
	enum MON_ANIM_CONDITION
	{
		AC_ANIMENDTRIGGER
		, AC_DETECTED
		, AC_ATTACKTRIGGER
		, AC_ISATTACK
		, AC_MOVE
		, AC_WALK
		, AC_RANDOM
		, AC_BORETRIGGER
		, AC_DAMAGETRIGGER
		, AC_HP
		, AC_STUN
		, AC_SKILL_ID
		, AC_LAST
	};

public:
	typedef struct MonsterDesc :public CCharacter::CHARACTER_DESC
	{
		MONSTER_ID eMonID;
		_vector vHomePos = { 10.f,1.f,10.f };
		CCubeTerrain* pCubeTerrain = { nullptr };
	}MONSTER_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Monster";


protected:
	explicit CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual _bool Check_Collision(CGameObject* pOther) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual void Hit(CGameObject* pFoe, _int fDamage) override;
	virtual void On_StateChange(_uint iState) override;
	virtual void On_SubStateChange(_uint iSubState) override;
	virtual void On_AnimEnd(_uint iAnimIdx) override;
	virtual void On_CastingEnd(CSkill* pSkill) override;
	virtual void On_HPZero() override;
	virtual _bool Use_Skill(CSkill* pSkill) override;
	virtual void Respawn();

	virtual _bool Is_Targetable() override;

	MONSTER_ID Get_MonsterID() { return m_pMonData->eMonID; }
	MONSTER_GRADE Get_MonsterGrade() { return m_pMonData->eMonGrade; }
	string Get_Name();
protected:
	HRESULT Ready_Components(MONSTER_DESC* pDesc);
	HRESULT Ready_Parts(MONSTER_DESC* pDesc);
	virtual HRESULT Ready_AnimStateMachine();
	virtual void To_NextSkill();

	void GoTo_Home(_float fTimeDelta);
	void Move_Random(_float fTimeDelta);
	void Move_To_Target(_float fTimeDelta);
	void Chase_Target(_float fTimeDelta);
	virtual _bool FindWay(_vector& vStart, _vector& vGoal, _uint iSearchRange);

public:
	virtual _int Get_AttackDamg() override;
	virtual _float Get_CritPefrcent() override;
	virtual  _float Get_HPRatio() override;
	virtual _int Get_TotalHP() override;
	_bool Is_AttackCoolReady() { return m_fAttackTimeAcc >= m_pMonData->tStat.fAttackInterval; }
protected:
	MONSTER_DATA* m_pMonData = { nullptr };


	_bool m_bDetected = { false };
	_bool m_bChase = { false };
	_bool m_bWalk = { false };
	_bool m_bStun = { false };

	_float m_fAttackTimeAcc{ 5.f };

	_float m_fDetectionRange{ 4.f };
	_float m_fHomeRange{ 6.f };
	_bool m_bBackToHome = { false };
	_float m_fChaseRange{ 2.f };

	_int m_iRandomCondition = { 0 };
	unordered_map<MON_STATE, vector<_uint>> m_mapAnimIdx;

	_float m_fRandomMoveTimeAcc = { 0.f };
	_float m_fRandomMoveTime = { 10.f };
	_bool m_bRandomMove = { false };
	_vector m_vRandomHomePosition = { 0.f,0.f,0.f,0.f };

	_float m_fDeadIdleTime = { 3.f };
	_float m_fDeadIdleTimeAcc = { 0.f };

	CWayFinder* m_pWayFinder = { nullptr };
	_uint m_iSearchRange = { 10 };
	_vector m_vNextStation = { 0.f,0.f,0.f,1.f };
	
	CCubeTerrain* m_pCubeTerrain = { nullptr };
	_int m_iTargetCubeIndex = { -1 } ;

	CWorldUIHPBar* m_pHPBar = { nullptr };

	_float m_fHPBarVisibleTime = { 5.f };
	_float m_fHPBarVisibleTimeAcc = { 0.f };
public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
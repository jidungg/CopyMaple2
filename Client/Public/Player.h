#pragma once
#include "Client_Defines.h"
#include "Character.h"



BEGIN(Client)
class CSkill;
class CBoneModelObject;
class CMimicBoneModelObject;
class CPlayer :
	public CCharacter
{
public:
	enum class ANIM_STATE {
		ATTACK_01
		,ATTACK_IDLE
		,BBQPARTY_1
		,BBQPARTY_2
		,BORE_A
		,BORE_B
		,BORE_C
		,CLIMB_DOWN_LAND
		,CLIMB_DOWN_TAKE
		,CLIMB_IDLE
		,CLIMB_L_DOWN
		,CLIMB_L_UP
		,CLIMB_LEFT
		,CLIMB_R_DOWN
		,CLIMB_R_UP
		,CLIMB_RIGHT
		,CLIMB_UP_LAND
		,CLIMB_UP_TAKE
		,CONCENTRATION
		,DAMG_A
		,DAMG_B
		,DAMG_C
		,DASH_JUMP
		,DEAD
		,DEAD2
		,DOWN_IDLE_A
		,DOWN_IDLE_B
		,DOWN_IDLE_C
		,DOWN_IDLE_D
		,EQUIP_CHANAGE_CAP
		,EQUIP_CHANGE_BODY
		,EQUIP_CHANGE_GLOVE
		,EQUIP_CHANGE_HAIR
		,EQUIP_CHANGE_HEAD
		,EQUIP_CHANGE_IDLE
		,EQUIP_CHANGE_MANTLE
		,EQUIP_CHANGE_SHOES
		,FAKEMETEOR_1
		,FAKEMETEOR_2
		,FIRETORNADO
		,FISHING_BORE_A
		,FISHING_BORE_B
		,FISHING_END
		,FISHING_IDLE
		,FISHING_REEL
		,FISHING_START
		,FLALMEBURST
		,FLAMEWAVE
		,FLY_IDLE
		,FLY_RUN
		,FUNCTOBJ_CHAIR
		,HOLD_IDLE
		,IDLE
		,JUMP_ATTACK
		,JUMP_DAMG
		,JUMP_DASH_DOWN
		,JUMP_DASH_FALL
		,JUMP_DASH_LAND
		,JUMP_DOWN_A
		,JUMP_DOWN_B
		,JUMP_FALL_A
		,JUMP_FALL_B
		,JUMP_FLYDAMG
		,JUMP_LAND
		,JUMP_UP_A
		,JUMP_UP_B
		,KINDLING_01
		,KINDLING_02
		,KINDLING_03_A
		,KINDLING_03_B
		,KNOCK_BACK
		,LADDER_DOWN_LAND
		,LADDER_DOWN_TAKE
		,LADDER_IDLE
		,LADDER_L_DOWN
		,LADDER_L_UP
		,LADDER_LEFT
		,LADDER_R_DOWN
		,LADDER_R_UP
		,LADDER_RIGHT
		,LADDER_UP_LAND
		,LADDER_UP_TAKE
		,MAGICCLAW
		,OBJECT_REACT_A
		,OBJECT_REACT_B
		,OBJECT_REACT_C
		,OBJECT_REACT_D
		,OBJECT_REACT_E
		,PROSTRATE
		,PROSTRATE_CRAWL_A
		,PROSTRATE_CRAWL_B
		,PROSTRATE_IDLE
		,PUSH_A
		,PUSH_B
		,RESURRECTION
		,RUN
		,SIT_CHAIR
		,SIT_CHAIR_BORE_A
		,SIT_CHAIR_BORE_B
		,SIT_CHAIR_IDLE
		,SIT_CHAIR_TALK
		,SIT_GROUND
		,SIT_GROUND_BORE_A
		,SIT_GROUND_BORE_B
		,SIT_GROUND_IDLE
		,SIT_GROUND_TALK
		,STAFF_ATTACK
		,STAFF_ATTACK_IDLE
		,STAFF_DAMG_A
		,STAFF_DAMG_B
		,STAFF_DEAD
		,STAFF_JUMP_ATTACK
		,STAFF_JUMP_DOWN_A
		,STAFF_JUMP_DOWN_B
		,STAFF_JUMP_LAND
		,STAFF_JUMP_UP_A
		,STAFF_JUMP_UP_B
		,STAFF_RUN
		,STAND_A
		,STAND_B
		,STAND_C
		,STUCK
		,STUN
		,STUN_FROZEN
		,TALK_A
		,TALK_B
		,TELEPORT
		,TRINITYFORCE
		,VOMIT
		,WALK
		,WILDFIRE_02
		,WIZARD_BORE_A
		,LAST
	};
	enum class ANIM_CONDITION
	{
		ANIM_END_TRIGGER,
		POSTDELAY_END,
		UPFORCE,
		HEIGHT,
		RANDOM,
		MOVE,
		IDLETIME,
		WALK,
		BATTLE,
		WEAPON,

		ATTACK_TRIGGER,
		SKILL_ID,
		LAST
	};
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{

	}PLAYER_DESC;

protected:
	explicit CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Receive_KeyInput(_float fTimeDelta) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Parts();
	HRESULT Ready_AnimStateMachine();
	HRESULT Ready_Skill();

	void On_StateChange(_uint iState);
	void On_SubStateChange(_uint iSubState);

	virtual void Use_Skill(CSkill* pSkill) override;

	CSkill* Get_Skill(SKILL_ID eID) { return m_pSkill[(_uint)eID]; }
	void Set_Battle(bool bBattle);
protected:

	CSkill* m_pSkill[(_uint)SKILL_ID::LAST];
	class CWeapon* m_pWeapon = { nullptr };
	CBoneModelObject* m_pHair = { nullptr };
	CMimicBoneModelObject* m_pRobe = { nullptr };

	unordered_map<SKILL_ID, ANIM_CONDITION> m_mapSkillTrigger;


	//ConditionVar
	_int m_iRandomCondition { 0 };
	_float m_fUpForce { 0.f };
	_float m_fHeight { 1.f };
	_float m_fIdleTime  { 0.f };
	_float m_fBattleTime { 0.f };
	_bool m_bWalk;
	_bool m_bMove { false };
	_bool m_bWeapon { false };
	_bool m_bBattle  { false };
	_bool m_bPostDelayEnd  { false };
	_int m_iSkillID{ 0 };
public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
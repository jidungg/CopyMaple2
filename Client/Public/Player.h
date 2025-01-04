#pragma once
#include "Client_Defines.h"
#include "Character.h"
#include "Item.h"


BEGIN(Client)
class CSkill;
class CBoneModelObject;
class CMimicBoneModelObject;
class CFace;
class CInventory;
class CItemObjet;
class CTerrainObject;
class CInteractableObject;
class CAttachableBodyPart;
class CBayar;
class CUIBundle;
class CUIQuickSlotBundle;
class CModelObject;
class CPlayer :
	public CCharacter
{
public:
	enum ANIM_STATE {
		AS_ATTACK_01
		,AS_ATTACK_IDLE
		,AS_BBQPARTY_1
		,AS_BBQPARTY_2
		,AS_BORE_A
		,AS_BORE_B
		,AS_BORE_C
		,AS_CLIMB_DOWN_LAND
		,AS_CLIMB_DOWN_TAKE
		,AS_CLIMB_IDLE 
		,AS_CLIMB_L_DOWN = 10
		,AS_CLIMB_L_UP
		,AS_CLIMB_LEFT
		,AS_CLIMB_R_DOWN
		,AS_CLIMB_R_UP
		,AS_CLIMB_RIGHT
		,AS_CLIMB_UP_LAND
		,AS_CLIMB_UP_TAKE
		,AS_CONCENTRATION
		,AS_DAMG_A
		,AS_DAMG_B
		,AS_DAMG_C
		,AS_DASH_JUMP
		,AS_DEAD
		,AS_DEAD2
		,AS_DOWN_IDLE_A
		,AS_DOWN_IDLE_B
		,AS_DOWN_IDLE_C
		,AS_DOWN_IDLE_D
		,AS_EQUIP_CHANAGE_CAP
		,AS_EQUIP_CHANGE_BODY
		,AS_EQUIP_CHANGE_GLOVE
		,AS_EQUIP_CHANGE_HAIR
		,AS_EQUIP_CHANGE_HEAD
		,AS_EQUIP_CHANGE_IDLE
		,AS_EQUIP_CHANGE_MANTLE
		,AS_EQUIP_CHANGE_SHOES
		,AS_FAKEMETEOR_1
		,AS_FAKEMETEOR_2
		,AS_FIRETORNADO
		,AS_FISHING_BORE_A
		,AS_FISHING_BORE_B
		,AS_FISHING_END
		,AS_FISHING_IDLE
		,AS_FISHING_REEL
		,AS_FISHING_START
		,AS_FLALMEBURST
		,AS_FLAMEWAVE = 47
		,AS_FLY_IDLE
		,AS_FLY_RUN
		,AS_FUNCTOBJ_CHAIR
		,AS_HOLD_IDLE
		,AS_IDLE = 52
		,AS_JUMP_ATTACK = 53
		,AS_JUMP_DAMG
		,AS_JUMP_DASH_DOWN
		,AS_JUMP_DASH_FALL
		,AS_JUMP_DASH_LAND
		,AS_JUMP_DOWN_A
		,AS_JUMP_DOWN_B
		,AS_JUMP_FALL_A = 60
		,AS_JUMP_FALL_B
		,AS_JUMP_FLYDAMG
		,AS_JUMP_LAND
		,AS_JUMP_UP_A = 64
		,AS_JUMP_UP_B
		,AS_KINDLING_01
		,AS_KINDLING_02
		,AS_KINDLING_03_A
		,AS_KINDLING_03_B
		,AS_KNOCK_BACK
		,AS_LADDER_DOWN_LAND
		,AS_LADDER_DOWN_TAKE
		,AS_LADDER_IDLE
		,AS_LADDER_L_DOWN
		,AS_LADDER_L_UP
		,AS_LADDER_LEFT
		,AS_LADDER_R_DOWN
		,AS_LADDER_R_UP
		,AS_LADDER_RIGHT
		,AS_LADDER_UP_LAND
		,AS_LADDER_UP_TAKE
		,AS_MAGICCLAW
		,AS_OBJECT_REACT_A
		,AS_OBJECT_REACT_B
		,AS_OBJECT_REACT_C
		,AS_OBJECT_REACT_D
		,AS_OBJECT_REACT_E
		,AS_PROSTRATE
		,AS_PROSTRATE_CRAWL_A
		,AS_PROSTRATE_CRAWL_B
		,AS_PROSTRATE_IDLE
		,AS_PUSH_A
		,AS_PUSH_B
		,AS_RESURRECTION
		,AS_RUN = 95
		,AS_SIT_CHAIR
		,AS_SIT_CHAIR_BORE_A
		,AS_SIT_CHAIR_BORE_B
		,AS_SIT_CHAIR_IDLE
		,AS_SIT_CHAIR_TALK
		,AS_SIT_GROUND
		,AS_SIT_GROUND_BORE_A
		,AS_SIT_GROUND_BORE_B
		,AS_SIT_GROUND_IDLE
		,AS_SIT_GROUND_TALK
		,AS_STAFF_ATTACK = 106
		,AS_STAFF_ATTACK_IDLE
		,AS_STAFF_DAMG_A
		,AS_STAFF_DAMG_B
		,AS_STAFF_DEAD
		,AS_STAFF_JUMP_ATTACK
		,AS_STAFF_JUMP_DOWN_A =112
		,AS_STAFF_JUMP_DOWN_B
		,AS_STAFF_JUMP_LAND
		,AS_STAFF_JUMP_UP_A
		,AS_STAFF_JUMP_UP_B
		,AS_STAFF_RUN
		,AS_STAND_A
		,AS_STAND_B
		,AS_STAND_C
		,AS_STUCK
		,AS_STUN
		,AS_STUN_FROZEN
		,AS_TALK_A
		,AS_TALK_B
		,AS_TELEPORT
		,AS_TRINITYFORCE
		,AS_VOMIT
		,AS_WALK
		,AS_WILDFIRE_01
		,AS_WIZARD_BORE_A
		,AS_LAST

		, BS_IDLE  = 1000
		, BS_ATTACK
		, BS_JUMP
		, BS_RUN
		,BS_CLIMB
		,BS_CLIMB_RUN = 1005
		, BS_CLIMB_LAND
		, BS_DAMAGED
		, BS_DEAD
		, BS_STUN
		, BS_LAST
	};
	enum ANIM_CONDITION
	{
		AC_ANIM_END_TRIGGER,
		AC_POSTDELAY_END,
		AC_JUMP_TRIGGER,
		AC_ONFLOOR,
		AC_UPFORCE,
		AC_RANDOM,
		AC_MOVE,
		AC_HPZERO,
		AC_ATTACHED,
		AC_IDLETIME,
		AC_WALK,
		AC_BATTLE,
		AC_WEAPON,
		AC_PAINTIME,
		AC_INTERACTION,
		AC_ATTACK,
		AC_BODYWALL,
		AC_FOOTWALL,
		AC_CLIMB,
		AC_CLIMB_DIR,
		AC_CLIMB_DIR_CHG,

		AC_CLIMB_LAND_TRIGGER,
		AC_ATTACK_TRIGGER,
		AC_SKILL_ID,
		AC_LAST
	};
	
public:
	typedef struct PlayerDesc : public CCharacter::CHARACTER_DESC
	{
		json jPlayerData;
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
	virtual _bool Check_Collision(CGameObject* pOther) override;

	virtual void On_StateChange(_uint iState) override;
	virtual void On_SubStateChange(_uint iSubState) override;
	virtual void On_AnimEnd(_uint iAnimIdx) override;
	virtual void On_CastingEnd(CSkill* pSkill) override;
	virtual void On_HPZero() override;
	void On_FaceStateChange(_uint iState);
	virtual void Attach_To(CAttachableBodyPart* pAttachablePart);
	virtual void Detach_From();
	virtual void Compute_Matrix() override;
	virtual _bool Is_Targetable() override;
public:

	HRESULT Ready_Parts(const json& jCustomData);
	HRESULT Ready_AnimStateMachine();
	HRESULT Ready_FaceStateMachine();
	HRESULT Ready_Skill(const json& jSkillData);
	HRESULT Ready_Stat(const json& jStatData);

	virtual _bool Use_Skill(CSkill* pSkill) override;

	HRESULT Gain_Item(const ITEM_DATA* pItem, _uint iCount = 1);


	void Set_Battle(bool bBattle); 
	HRESULT Equip(const EQUIP_ITEM_DATA* pItem);
	HRESULT UnEquip(EQUIP_ITEM_TYPE eType);
	HRESULT Equip(const DECO_ITEM_DATA* pItem);
	HRESULT UnEquip(DECO_ITEM_TYPE eType);
	HRESULT Set_Customize(struct CUSTOMIZE_DESC* pItem);
	void Set_BodyMeshActive(EQUIP_ITEM_TYPE eType, bool bActive);
	_bool Is_Weapon() { return m_bWeapon; }

	virtual _int Get_AttackDamg() override;
	virtual _float Get_CritPefrcent() override;
	virtual  _float Get_HPRatio() override;
	virtual _int Get_TotalHP() override;
protected:
	CUIQuickSlotBundle* m_pQuickSlotBundle = { nullptr };
	CUIQuickSlotBundle* m_pQuickSlotBundle2 = { nullptr };
	CInteractableObject* m_pInteractable = { nullptr };

	CModelObject* m_pEquipModels[(_uint)EQUIP_ITEM_TYPE::LAST] = { nullptr, };
	CModelObject* m_pDecoModels[(_uint)DECO_ITEM_TYPE::LAST] = { nullptr, };
	CModelObject* m_pCustomizes[(_uint)CUSTOMIZE_PART::LAST] = { nullptr, };


	CStateMachine* m_pFaceStateMachine = { nullptr };

	CInventory* m_pInventory = { nullptr };

	_float m_fPainTime = 0.f;


	//ConditionVar
	_int m_iRandomCondition { 0 };

	_float m_fIdleTime  { 0.f };

	_float m_fBattleTime { 0.f };
	_bool m_bWalk{ false };
	_bool m_bClimb{ false };
	_bool m_bBodyWall{ false };
	_bool m_bFootWall{ false };
	_bool m_bWeapon { false };
	_bool m_bBattle  { false };
	_bool m_bPostDelayEnd  { false };

	_bool m_bInteraction{ false };
	_float m_fPainTimeAcc;
	_int m_iClimbDir{ (_int)DIR_U };
	_vector m_vBodyWallNormal{ 0.f, 0.f, 0.f ,0.f};
	_vector m_vBodyWallPoint{ 0.f, 0.f, 0.f ,0.f };
	_float m_fBodyWallHeight{ 0.f };

	//ATTACH
	_bool m_bAttached = { false };
	CAttachableBodyPart* m_pAttachedObject = { false };
	_matrix m_matAttachOffset;


public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
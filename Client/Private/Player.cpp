#include "stdafx.h"
#include "Player.h"
#include "Client_Utility.h"
#include "ModelObject.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "SkillManager.h"
#include "Weapon.h"
#include "Face.h"
#include "HumanModelObject.h"
#include "Item.h"
#include "Inventory.h"
#include "ItemDataBase.h"
#include "UIBundle.h"
#include "CubeTerrain.h"
#include "Collider_Sphere.h"
#include "DeadObjEvent.h"
#include "Interactable.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
	for (auto& pEquip : m_pEquipModels)
	{
		pEquip = nullptr;
	}
	for (auto& pDeco : m_pDecoModels)
	{
		pDeco = nullptr;
	}
	for (auto& pCustomize : m_pCustomizes)
	{
		pCustomize = nullptr;
	}
	m_eTeam = TEAM::PLAYER;
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CCharacter(Prototype)
{

	Safe_AddRef(m_pAnimStateMachine);
	m_pBody = Prototype.m_pBody;
	Safe_AddRef(m_pBody);
	m_eTeam = TEAM::PLAYER;


	m_fFloorHeight = Prototype.m_fFloorHeight;
	m_fUpForce = Prototype.m_fUpForce;
	m_vMoveDirectionXZ = Prototype.m_vMoveDirectionXZ;
	m_iRandomCondition = Prototype.m_iRandomCondition;
	m_fIdleTime = Prototype.m_fIdleTime;
	m_bWalk = Prototype.m_bWalk;
	m_bMove = Prototype.m_bMove;
	m_bWeapon = Prototype.m_bWeapon;
	m_bBattle = Prototype.m_bBattle;
	m_fBattleTime = Prototype.m_fBattleTime;
	m_bPostDelayEnd = Prototype.m_bPostDelayEnd;
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_pInventory = INVENTORY;
	PLAYER_DESC* desc = static_cast<PLAYER_DESC*>(pArg);
	desc->fRotationPerSec = 5.f;
	desc->fBodyCollisionRadius = 0.125f;
	desc->fBodyCollisionOffset = { 0, 0.4f, 0 };
	desc->iBodyColliderIndex = 0;
	desc->iColliderCount = 1;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	if (FAILED(Ready_AnimStateMachine()))
		return E_FAIL;
	if (FAILED(Ready_FaceStateMachine()))
		return E_FAIL;
	if (FAILED(Ready_Skill(desc->jPlayerData["Skill"])))
		return E_FAIL;
	if (FAILED(Ready_Stat(desc->jPlayerData["Status"])))
		return E_FAIL;


	ITEM_DATA* pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::BASIC_STAFF);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::BASIC_HAT);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::BASIC_SHOES);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::BASIC_SUIT);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::BASIC_SUIT);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::EVILWINGS);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::FIREPRISM_BOTTOM);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::FIREPRISM_CAPE);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::FIREPRISM_EAR);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::FIREPRISM_GLOVE);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::FIREPRISM_HAT);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::FIREPRISM_STAFF);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::FIREPRISM_TOP);
	Gain_Item(pItemDesc);
	pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::FIREPRISM_SHOES);
	Gain_Item(pItemDesc);

	UIBUNDLE->Set_QuickItem(KEY::Q, m_mapSkill[SKILL_ID::TELEPORT]);
	UIBUNDLE->Set_QuickItem(KEY::W, m_mapSkill[SKILL_ID::BBQ_PARTY]);
	UIBUNDLE->Set_QuickItem(KEY::E, m_mapSkill[SKILL_ID::FAKE_METEOR]);
	UIBUNDLE->Set_QuickItem(KEY::R, m_mapSkill[SKILL_ID::WILD_FIRE]);
	UIBUNDLE->Set_QuickItem(KEY::A, m_mapSkill[SKILL_ID::MAGIC_CLAW]);
	UIBUNDLE->Set_QuickItem(KEY::S, m_mapSkill[SKILL_ID::KINDLING]);
	UIBUNDLE->Set_QuickItem(KEY::D, m_mapSkill[SKILL_ID::FLAME_WAVE]);
	UIBUNDLE->Set_QuickItem(KEY::F, m_mapSkill[SKILL_ID::FIRE_TORNADO]);
	return S_OK;
}

HRESULT CPlayer::Ready_Parts()
{
	//Body
	CHumanModelObject::HUMANMODELOBJ_DESC tHumanModelDesc = {};

	tHumanModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tHumanModelDesc.strModelProtoName, "Female_Wizard.model");
	tHumanModelDesc.szFaceProtoTag = TEXT("Face_Face1");
	m_pBody = static_cast<CHumanModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CHumanModelObject::m_szProtoTag, &tHumanModelDesc));
	Add_Child(m_pBody);
	m_pBody->Set_Animation(ANIM_STATE::AS_IDLE);

	//Hair
	CBoneModelObject::BONEMODELOBJ_DESC tBoneModelDesc = {};
	tBoneModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tBoneModelDesc.strModelProtoName, "00200028_m_shortpigtailhair_p_a.model");
	tBoneModelDesc.pSocketMatrix = (m_pBody)->Get_BoneMatrix("HR");//Bip01 HeadNub_end
	m_pCustomizes[(_uint)CUSTOMIZE_PART::HAIR] = static_cast<CBoneModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CBoneModelObject::m_szProtoTag, &tBoneModelDesc));
	if (nullptr == m_pCustomizes[(_uint)CUSTOMIZE_PART::HAIR]) return E_FAIL;
	Add_Child(m_pCustomizes[(_uint)CUSTOMIZE_PART::HAIR]);
	m_pCustomizes[(_uint)CUSTOMIZE_PART::HAIR]->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));


	//FaceDeco
	tBoneModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tBoneModelDesc.strModelProtoName, "01000001_c_lolipop.model");
	tBoneModelDesc.pSocketMatrix = (m_pBody)->Get_BoneMatrix("Bip01 Head");//Bip01 HeadNub_end
	m_pDecoModels[(_uint)DECO_TYPE::FACE] = static_cast<CBoneModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CBoneModelObject::m_szProtoTag, &tBoneModelDesc));
	if (nullptr == m_pDecoModels[(_uint)DECO_TYPE::FACE]) return E_FAIL;
	Add_Child(m_pDecoModels[(_uint)DECO_TYPE::FACE]);
	m_pDecoModels[(_uint)DECO_TYPE::FACE]->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));



	return S_OK;
}



HRESULT CPlayer::Ready_AnimStateMachine()
{
	////AnimStateMachine
	//m_pAnimStateMachine = static_cast<CStateMachine*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Prototype_GameObject_StateMachine")));
	//Add_Component(m_pAnimStateMachine, TEXT("Com_StateMachine"));
	//m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CPlayer::On_StateChange, this, placeholders::_1));
	//m_pAnimStateMachine->Register_OnSubStateChangeCallBack(bind(&CPlayer::On_SubStateChange, this, placeholders::_1));
	m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CPlayer::On_StateChange, this, placeholders::_1));
	m_pAnimStateMachine->Register_OnSubStateChangeCallBack(bind(&CPlayer::On_SubStateChange, this, placeholders::_1));
	m_pBody->Register_OnAnimEndCallBack(bind(&CPlayer::On_AnimEnd, this, placeholders::_1));

	CTransition* pTransition; 
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_IDLE, true);
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_ATTACK_IDLE, true);
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_RUN, true);
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_WALK, true);
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_STAFF_RUN, true);
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_STAFF_ATTACK_IDLE, true);
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_CLIMB_IDLE, true);
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_CLIMB_LEFT, true);
	m_pBody->Set_AnimationLoop(ANIM_STATE::AS_CLIMB_RIGHT, true);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_ATTACK_01, 0.8f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_JUMP_ATTACK, 1.0f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_STAFF_ATTACK, 0.5f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_STAFF_JUMP_ATTACK, 1.0f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_MAGICCLAW, 0.5f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_KINDLING_02, 0.5f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_WILDFIRE_01, 1.f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_BBQPARTY_2, 1.f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_FAKEMETEOR_2, 1.f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_TELEPORT, 1.f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_FLAMEWAVE, 0.5f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_FIRETORNADO, 1.f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_CLIMB_UP_LAND, 1.f);
	m_pBody->Set_AnimPostDelayPercent(ANIM_STATE::AS_CLIMB_DOWN_LAND, 1.f);

	//CONDITION
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_ANIM_END_TRIGGER));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_ATTACK_TRIGGER));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_CLIMB_DIR_CHG));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_CLIMB_LAND_TRIGGER));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::AC_JUMP_TRIGGER));

	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_RANDOM), &m_iRandomCondition);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_ONFLOOR), &m_bOnFloor);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_IDLETIME), &m_fIdleTime);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_UPFORCE), &m_fUpForce);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_WALK), &m_bWalk);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_MOVE), &m_bMove);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_WEAPON), &m_bWeapon);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_BATTLE), &m_bBattle);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_POSTDELAY_END), &m_bPostDelayEnd);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_SKILL_ID), &m_iCurrentSkillID);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_ATTACK), &m_bAttack);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_CLIMB), &m_bClimb);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_CLIMB_DIR), &m_iClimbDir);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_BODYWALL), &m_bBodyWall);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_FOOTWALL), &m_bFootWall);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_HPZERO), &m_bHPZero);


	//STATE
	for (_uint i = 0; i < ANIM_STATE::AS_LAST; i++)
		m_pAnimStateMachine->Add_State(i);
	for (_uint i = BS_IDLE; i < ANIM_STATE::BS_LAST; i++)
		m_pAnimStateMachine->Add_State(i);

#pragma region MainTransition

	//Transition
	//IDLE
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_IDLE, ANIM_STATE::BS_ATTACK);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ATTACK_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_IDLE, ANIM_STATE::BS_JUMP);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_JUMP_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_IDLE, ANIM_STATE::BS_RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_IDLE, ANIM_STATE::BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_HPZERO, CONDITION_TYPE::EQUAL, true);

	//ATTACK
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_ATTACK, ANIM_STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACK,CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_ATTACK, ANIM_STATE::BS_JUMP);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ONFLOOR, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_ATTACK, ANIM_STATE::BS_RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_POSTDELAY_END, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ONFLOOR, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_ATTACK, ANIM_STATE::BS_ATTACK);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_POSTDELAY_END, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ATTACK_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_ATTACK, ANIM_STATE::BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_HPZERO, CONDITION_TYPE::EQUAL, true);

	//JUMP
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_JUMP, ANIM_STATE::BS_ATTACK);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ATTACK_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_JUMP, ANIM_STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ONFLOOR, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_JUMP, ANIM_STATE::BS_RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ONFLOOR, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_JUMP, ANIM_STATE::BS_CLIMB);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_JUMP, ANIM_STATE::BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_HPZERO, CONDITION_TYPE::EQUAL, true);

	//RUN
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_RUN, ANIM_STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_RUN, ANIM_STATE::BS_ATTACK);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ATTACK_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_RUN, ANIM_STATE::BS_JUMP);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_JUMP_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_RUN, ANIM_STATE::BS_JUMP);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ONFLOOR, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_RUN, ANIM_STATE::BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_HPZERO, CONDITION_TYPE::EQUAL, true);

	//CLIMB
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB, ANIM_STATE::BS_CLIMB_RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB, ANIM_STATE::BS_JUMP);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_JUMP_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB, ANIM_STATE::BS_CLIMB_LAND);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_CLIMB_LAND_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB, ANIM_STATE::BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_HPZERO, CONDITION_TYPE::EQUAL, true);

	//CLIMB_RUN
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::BS_JUMP);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::BS_JUMP);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_JUMP_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::BS_JUMP);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BODYWALL, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_FOOTWALL, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::BS_CLIMB);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::BS_CLIMB_RUN);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR_CHG);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::BS_CLIMB_LAND);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_CLIMB_LAND_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_HPZERO, CONDITION_TYPE::EQUAL, true);

	//CLIMB_LAND
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_LAND, ANIM_STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_CLIMB_LAND, ANIM_STATE::BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_HPZERO, CONDITION_TYPE::EQUAL, true);

	//DEAD_
	pTransition = m_pAnimStateMachine->Add_Transition(ANIM_STATE::BS_DEAD, ANIM_STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_HPZERO, CONDITION_TYPE::EQUAL, false);

#pragma endregion

#pragma region SubState
	//SUBSTATE
//IDLE  _SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_IDLE, ANIM_STATE::AS_IDLE);

	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_IDLE, ANIM_STATE::AS_STAFF_ATTACK_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_IDLE, ANIM_STATE::AS_ATTACK_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_IDLE, ANIM_STATE::AS_CLIMB_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_ATTACK_IDLE, ANIM_STATE::AS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_STAFF_ATTACK_IDLE, ANIM_STATE::AS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, false);

	//RUN _SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_RUN, ANIM_STATE::AS_RUN);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_RUN, ANIM_STATE::AS_STAFF_RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_RUN, ANIM_STATE::AS_WALK);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WALK, CONDITION_TYPE::EQUAL, true);

	//CLIMB _SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_CLIMB, ANIM_STATE::AS_CLIMB_IDLE);
	
	//CLIMB_LAND _SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_CLIMB_LAND, ANIM_STATE::AS_CLIMB_UP_LAND);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BODYWALL, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_FOOTWALL, CONDITION_TYPE::EQUAL, true);

	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_CLIMB_LAND, ANIM_STATE::AS_CLIMB_DOWN_LAND);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BODYWALL, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_FOOTWALL, CONDITION_TYPE::EQUAL, true);

	//CLIMB_RUN _SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::AS_CLIMB_L_UP);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR, CONDITION_TYPE::EQUAL, (_int)DIR_U);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::AS_CLIMB_R_DOWN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR, CONDITION_TYPE::EQUAL, (_int)DIR_D);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::AS_CLIMB_RIGHT);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR, CONDITION_TYPE::EQUAL, (_int)DIR_E);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_CLIMB_RUN, ANIM_STATE::AS_CLIMB_LEFT);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR, CONDITION_TYPE::EQUAL, (_int)DIR_W);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_CLIMB_L_UP, ANIM_STATE::AS_CLIMB_R_UP);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR, CONDITION_TYPE::EQUAL, (_int)DIR_U);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_CLIMB_R_UP, ANIM_STATE::AS_CLIMB_L_UP);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR, CONDITION_TYPE::EQUAL, (_int)DIR_U);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_CLIMB_L_DOWN, ANIM_STATE::AS_CLIMB_R_DOWN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR, CONDITION_TYPE::EQUAL, (_int)DIR_D);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_CLIMB_R_DOWN, ANIM_STATE::AS_CLIMB_L_DOWN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_CLIMB_DIR, CONDITION_TYPE::EQUAL, (_int)DIR_D);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	
	//JUMP SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_JUMP, ANIM_STATE::AS_JUMP_UP_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::LESS, 50);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_JUMP_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_JUMP, ANIM_STATE::AS_JUMP_UP_B);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_JUMP_TRIGGER);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::EQUAL_GREATER, 50);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_JUMP, ANIM_STATE::AS_JUMP_DOWN_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_UPFORCE, CONDITION_TYPE::EQUAL_LESS, 0);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_JUMP, ANIM_STATE::AS_STAFF_JUMP_DOWN_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_UPFORCE, CONDITION_TYPE::EQUAL_LESS, 0);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::LESS, 0);

	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_JUMP_UP_A, ANIM_STATE::AS_JUMP_DOWN_A);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_JUMP_UP_B, ANIM_STATE::AS_JUMP_DOWN_B);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);

	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_JUMP, ANIM_STATE::AS_STAFF_JUMP_UP_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_JUMP_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_JUMP, ANIM_STATE::AS_STAFF_JUMP_UP_B);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_JUMP_TRIGGER);

	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_STAFF_JUMP_UP_A, ANIM_STATE::AS_STAFF_JUMP_DOWN_A);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_STAFF_JUMP_UP_B, ANIM_STATE::AS_STAFF_JUMP_DOWN_B);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);

	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_JUMP_DOWN_A, ANIM_STATE::AS_JUMP_LAND);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_JUMP_DOWN_B, ANIM_STATE::AS_JUMP_LAND);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);	
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_STAFF_JUMP_DOWN_A, ANIM_STATE::AS_STAFF_JUMP_LAND);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_STAFF_JUMP_DOWN_B, ANIM_STATE::AS_STAFF_JUMP_LAND);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	//ATTACK_SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_ATTACK_01);

	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_JUMP_ATTACK);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ONFLOOR, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::BASIC_ATTACK);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_STAFF_ATTACK);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::BASIC_ATTACK);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_STAFF_JUMP_ATTACK);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ONFLOOR, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::BASIC_ATTACK);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_MAGICCLAW);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::MAGIC_CLAW);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_TELEPORT);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::TELEPORT);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_FLAMEWAVE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::FLAME_WAVE);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_FIRETORNADO);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::FIRE_TORNADO);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_KINDLING_01);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::KINDLING);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_KINDLING_01, ANIM_STATE::AS_KINDLING_02);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_BBQPARTY_1);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::BBQ_PARTY);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_BBQPARTY_1, ANIM_STATE::AS_BBQPARTY_2);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_FAKEMETEOR_1);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::FAKE_METEOR);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_FAKEMETEOR_1, ANIM_STATE::AS_FAKEMETEOR_2);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_ATTACK, ANIM_STATE::AS_WILDFIRE_01);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::WILD_FIRE);
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::AS_WILDFIRE_01, ANIM_STATE::AS_TELEPORT);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::WILD_FIRE2);

	//DEAD_SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(ANIM_STATE::BS_DEAD, ANIM_STATE::AS_STAFF_DEAD);

#pragma endregion

	m_pAnimStateMachine->Set_CurrentState(ANIM_STATE::BS_IDLE);
	return S_OK;
}

HRESULT CPlayer::Ready_FaceStateMachine()
{
	//AnimStateMachine
	m_pFaceStateMachine = static_cast<CStateMachine*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Prototype_GameObject_StateMachine")));
	Add_Component(m_pFaceStateMachine, TEXT("Com_FaceStateMachine"));
	m_pFaceStateMachine->Register_OnStateChangeCallBack(bind(&CPlayer::On_FaceStateChange, this, placeholders::_1));

	m_pFaceStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_BATTLE), &m_bBattle);
	m_pFaceStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_INTERACTION), &m_bInteraction);
	m_pFaceStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::AC_PAINTIME), &m_fPainTimeAcc);

	//STATE ADD
	for (_uint i = 0; i < (_uint)CFace::FACE_STATE::LAST; i++)
		m_pFaceStateMachine->Add_State(i);

	//IDLE
	CTransition* pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::IDLE, (_uint)CFace::FACE_STATE::ATTACK);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::IDLE, (_uint)CFace::FACE_STATE::FOCUS);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_INTERACTION, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::IDLE, (_uint)CFace::FACE_STATE::PAIN);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_PAINTIME, CONDITION_TYPE::EQUAL_LESS, m_fPainTime);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_PAINTIME, CONDITION_TYPE::GREATER, 0.0f);

	//ATTACK
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::ATTACK, (_uint)CFace::FACE_STATE::IDLE);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_BATTLE, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::ATTACK, (_uint)CFace::FACE_STATE::PAIN);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_PAINTIME, CONDITION_TYPE::EQUAL_LESS, m_fPainTime);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_PAINTIME, CONDITION_TYPE::GREATER, 0.0f);

	//PAIN
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::PAIN, (_uint)CFace::FACE_STATE::ATTACK);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_PAINTIME, CONDITION_TYPE::EQUAL_GREATER, m_fPainTime);

	//FOCUS
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::FOCUS, (_uint)CFace::FACE_STATE::IDLE);
	m_pFaceStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_INTERACTION, CONDITION_TYPE::EQUAL, false);

	m_pFaceStateMachine->Set_CurrentState((_uint)CFace::FACE_STATE::IDLE);
	return S_OK;
}



HRESULT CPlayer::Ready_Skill(const json& jSkillData)
{
	CSkillDataBase* pSkillManager = SKILLDB;
	vector<SKILL_ID> vecSkillID = jSkillData.get<vector<SKILL_ID>>();
	for (auto& eSkillID : vecSkillID)
	{
		m_mapSkill[eSkillID] = CSkill::Create(pSkillManager->Get_SkillData(eSkillID), this);
		m_mapSkill[eSkillID]->Register_AnimEvent(m_pBody);
	}

	return S_OK;
}

HRESULT CPlayer::Ready_Stat(const json& jStatData)
{
	m_tStatDefault = Stat(jStatData);
	m_tStat = m_tStatDefault;
	return S_OK;
}



void CPlayer::Receive_KeyInput(_float fTimeDelta)
{

	ANIM_STATE eCurrentState = ANIM_STATE(m_pAnimStateMachine->Get_CurrentState());


	if (m_pGameInstance->GetKeyState(KEY::LSHIFT) == KEY_STATE::DOWN)
		m_bWalk = true;
	else if (m_pGameInstance->GetKeyState(KEY::LSHIFT) == KEY_STATE::UP)
		m_bWalk = false;


	_vector vAddDir{ 0,0,0,0 };
	_float vMoveSpeed = m_tStat.fRunSpeed;
	if (m_bClimb  )
	{
		if (m_pBody->Is_AnimPostDelayEnd())
		{
			_int iDirBefore = m_iClimbDir;
			if (m_pGameInstance->GetKeyState(KEY::RIGHT) == KEY_STATE::PRESSING)
			{
				vAddDir += m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
				m_iClimbDir = (_int)DIR_E;
			}
			if (m_pGameInstance->GetKeyState(KEY::LEFT) == KEY_STATE::PRESSING)
			{
				vAddDir -= m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
				m_iClimbDir = (_int)DIR_W;
			}
			if (m_pGameInstance->GetKeyState(KEY::UP) == KEY_STATE::PRESSING)
			{
				vAddDir += m_pTransformCom->Get_State(CTransform::STATE_UP);
				m_iClimbDir = (_int)DIR_U;
			}
			if (m_pGameInstance->GetKeyState(KEY::DOWN) == KEY_STATE::PRESSING)
			{
				vAddDir -= m_pTransformCom->Get_State(CTransform::STATE_UP);
				m_iClimbDir = (_int)DIR_D;
			}
			if (iDirBefore != m_iClimbDir)
				m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_CLIMB_DIR_CHG);

		}
		else
		{
			vAddDir = m_vLookDirectionXZ + XMVectorSet(0, 1, 0, 0);
			vMoveSpeed = 10.f;
		}
	}
	else
	{
		if (m_pGameInstance->GetKeyState(KEY::RIGHT) == KEY_STATE::PRESSING)
			vAddDir += Get_Direction_Vector(DIR_E);
		if (m_pGameInstance->GetKeyState(KEY::LEFT) == KEY_STATE::PRESSING)
			vAddDir += Get_Direction_Vector(DIR_W);
		if (m_pGameInstance->GetKeyState(KEY::UP) == KEY_STATE::PRESSING)
			vAddDir += Get_Direction_Vector(DIR_N);
		if (m_pGameInstance->GetKeyState(KEY::DOWN) == KEY_STATE::PRESSING)
			vAddDir += Get_Direction_Vector(DIR_S);
	}


	//이하 딜레이 끝나면 누를 수 있는 키들
	if (m_pBody->Is_AnimPostDelayEnd())
	{
		if (m_pGameInstance->GetKeyState(KEY::X) == KEY_STATE::DOWN)
		{
			m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_ATTACK_TRIGGER);
			m_iCurrentSkillID =(_int) SKILL_ID::BASIC_ATTACK;
			m_bAttack = true;
			Set_Battle(true);
		}
		if (m_pGameInstance->GetKeyState(KEY::C) == KEY_STATE::DOWN)
			if (m_fUpForce == 0.f)
			{
				m_bClimb = false;
				m_bOnFloor = false;
				m_fUpForce = m_tStat.fJumpPower;
				m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_JUMP_TRIGGER);
			}
		if (m_pGameInstance->GetKeyState(KEY::SPACE) == KEY_STATE::DOWN)
		{
			if (m_pInteractable)
				m_pInteractable->Interact(this);
		}
	}
	else
	{
		if (m_bOnFloor)
			vAddDir = XMVectorZero();
	}
	vAddDir = XMVector3Normalize(vAddDir);
	m_vMoveDirectionXZ += vAddDir * vMoveSpeed * fTimeDelta;
	if (false == m_bClimb)
		m_vLookDirectionXZ = vAddDir;
	else
		m_vLookDirectionXZ = -m_vBodyWallNormal;
}
void CPlayer::Priority_Update(_float fTimeDelta)
{
	
	__super::Priority_Update(fTimeDelta);
}
void CPlayer::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);
}



_bool CPlayer::Check_Collision(CGameObject* pOther)
{
	assert(pOther->Is_Active());
	assert(false == pOther->Is_Dead());

	__super::Check_Collision(pOther);

	LAYERID eLayerID = (LAYERID)pOther->Get_LayerID();
	switch (eLayerID)
	{
	case Client::LAYER_MONSTER:
	{
		break;
	}
	case Client::LAYER_INTERACTION:
	{
		CInteractableObject* pInteractable = static_cast<CInteractableObject*>(pOther);
		if (pInteractable->Check_Collision(this))
		{
			if (pInteractable->Is_InteractionPossible(this))
			{
				//이미 상호작용 가능한 오브젝트가 있으면?
				if (m_pInteractable && m_pInteractable != pInteractable)
				{
					// 누가 더 가까운지 판단하기 -> 더 가까운 오브젝트로 교체
					if (m_pInteractable->Get_Distance(this) > pOther->Get_Distance(this))
					{
						Safe_Release(m_pInteractable);
						m_pInteractable = pInteractable;
						Safe_AddRef(m_pInteractable);
					}
				}
				else
				{
					Safe_Release(m_pInteractable);
					m_pInteractable = pInteractable;
					Safe_AddRef(m_pInteractable);
				}
			}
		}
		else
		{
			Safe_Release(m_pInteractable);
			m_pInteractable = nullptr;
		}
	

		break;
	}
	case Client::LAYER_TERRAIN:
	{

		CCubeTerrain* pTerrain = static_cast<CCubeTerrain*>(pOther);
		_vector vFootPos = Get_Position();
		_vector vBodyPos = XMVectorSetY(vFootPos, vFootPos.m128_f32[1] + Get_BodyCollisionOffset().y);
		_float fCollisionRadius = Get_BodyCollisionRadius();
		Ray tBodyRay(vBodyPos, m_vLookDirectionXZ, fCollisionRadius);
		RaycastHit tBodyhit;
		m_bBodyWall = pTerrain->RayCastXZ(tBodyRay, &tBodyhit);
		Ray tFootRay(vFootPos, m_vLookDirectionXZ, fCollisionRadius);
		RaycastHit tFoothit;
		m_bFootWall = pTerrain->RayCastXZ(tFootRay, &tFoothit);
		if(m_bBodyWall)
		{
			m_vBodyWallNormal = XMVector3Normalize(tBodyhit.vNormal);
			m_fBodyWallHeight = static_cast<CTerrainObject*>( tBodyhit.pCollider->Get_Owner())->Get_TopHeight(vBodyPos);
			m_vBodyWallPoint = tBodyhit.vPoint;
		}

		break;
	}
	case Client::LAYER_PLAYER:
	case Client::LAYER_UI:
	case Client::LAYER_CAMERA:
	case Client::LAYER_LAST:
	default:
		break;
	}
	return S_OK;
}

void CPlayer::Late_Update(_float fTimeDelta)
{

	m_iRandomCondition = rand() % 100;


	if (m_pAnimStateMachine->Get_CurrentState() == ANIM_STATE::AS_IDLE)
		m_fIdleTime += fTimeDelta;
	if (m_bBattle)
	{
		m_fBattleTime += fTimeDelta;
		if (m_fBattleTime >= m_fBattleTimeMax)
		{
			Set_Battle(false);
		}
	}

	m_bPostDelayEnd = m_pBody->Is_AnimPostDelayEnd();

	//이하 Character 로직


	m_vNextPos = XMVectorSetY(m_vNextPos, XMVectorGetY(m_vNextPos) + m_fUpForce * fTimeDelta);
	_float fFootHeight = XMVectorGetY(m_vNextPos);
	_float fColliderHeight = Get_BodyCollisionOffset().y;
	if (m_fCelingHeight <= fFootHeight + fColliderHeight)
	{
		m_fUpForce = 0;
		m_vNextPos = XMVectorSetY(m_vNextPos, m_fCelingHeight - fColliderHeight);
	}

	//그냥 걸어가다가 떨어지는 건 
	if (m_bOnFloor)
	{

		if (XMVectorGetY(m_vNextPos) - m_fFloorHeight >= 0.2f)
			m_bOnFloor = false;
	}
	else
		m_bOnFloor = XMVectorGetY(m_vNextPos) <= m_fFloorHeight;
	if (m_bClimb)//벽 타는 중
	{
		//위로 나갔는지, 옆으로 나갔는지, 내려갔는지 판단
		if (false == m_bBodyWall)
		{
			if (m_bFootWall)//위로 나감
			{
				m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_CLIMB_LAND_TRIGGER);

			}
			else//옆으로 나감
			{
				m_fUpForce = 0;
				m_bClimb = false;
			}
		}
		else if (m_bOnFloor)//내려감
		{
			m_fUpForce = 0;
			m_vNextPos = XMVectorSetY(m_vNextPos, m_fFloorHeight);
			m_bClimb = false;
			m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_CLIMB_LAND_TRIGGER);
		}
		else//벽 타는 중
		{

			m_fUpForce = 0;
		}
	}
	else//벽 안타는 중
	{
		if (m_bOnFloor)//그냥 바닥에 있는중
		{
			if (m_fUpForce < 0)
			{
				m_fUpForce = 0;
			}
			m_vNextPos = XMVectorSetY(m_vNextPos, m_fFloorHeight);
		}
		else//공중에 있는중
		{
			if (m_bBodyWall && m_fUpForce <= 0)//벽에 부딫힘
			{
				m_bClimb = true;
				m_fUpForce = 0;
			}
			else//그냥 공중임
				m_fUpForce -= fTimeDelta * 9.8f * 3;
		}

	}

	if (m_bMove)
	{
		if (m_bClimb)
			m_pTransformCom->LookToward(XMVectorSetY(-m_vBodyWallNormal, 0));
		else
			m_pTransformCom->LookToward(XMVectorSetY(m_vLookDirectionXZ, 0));

		Get_CurrentSkill()->Cancel_Casting();
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vNextPos);

	m_fMoveDistanceXZ = 0.f;
	m_vMoveDirectionXZ = XMVectorSet(0, 0, 0, 0);
	Get_CurrentSkill()->Late_Update(fTimeDelta);
	CPawn::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}
void CPlayer::On_StateChange(_uint iState)
{
	if (iState == ANIM_STATE::AS_IDLE)
	{
		m_fIdleTime = 0.f;
	}

}

void CPlayer::On_SubStateChange(_uint iSubState)
{
	m_pBody->Switch_Animation(iSubState);

}
void CPlayer::On_AnimEnd(_uint iAnimIdx)
{
	m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_ANIM_END_TRIGGER);

	if (iAnimIdx == ANIM_STATE::AS_CLIMB_DOWN_LAND || iAnimIdx == ANIM_STATE::AS_CLIMB_UP_LAND)
	{
		m_bClimb = false;
		m_bOnFloor = true;
	}
	else if(m_bAttack)
	{
		_int iNextAnim = m_mapSkill[(SKILL_ID)m_iCurrentSkillID]->Get_NextAnimation(iAnimIdx);
		if (iNextAnim == -1)//스킬 종료
		{
			m_bAttack = false;
		}
	}
}

void CPlayer::On_CastingEnd(CSkill* pSkill)
{
	UIBUNDLE->Set_CastingBarVisible(false);
}

void CPlayer::On_FaceStateChange(_uint iState)
{
	static_cast<CHumanModelObject*>(m_pBody)->Set_FaceState((CFace::FACE_STATE)iState);
}

HRESULT CPlayer::Render()
{
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active() && child->Is_Dead() == false)
			child->Render();
	}
	Get_CurrentSkill()->Render_Collider();
	return S_OK;
}

_bool CPlayer::Use_Skill(CSkill* pSkill)
{
	if (pSkill == nullptr)
		return false ;
	if (false == Get_CurrentSkill()->Is_CastingComplete())
		return false;
	if (false == m_pBody->Is_AnimPostDelayEnd())
		return false;
	auto pDesc = pSkill->Get_SkillDesc();
	if (pDesc->eCastingType == SKILL_CASTING_TYPE::CASTING && false == m_bOnFloor)
		return false;


	m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_ATTACK_TRIGGER);
	Set_Battle(true);
	m_bAttack = true;
	m_iCurrentSkillID = (_int)pDesc->eID;
	if (pDesc->eCastingType == SKILL_CASTING_TYPE::CASTING)
		UIBUNDLE->Set_CastingBarVisible(true);
	return true;
}

HRESULT CPlayer::Gain_Item(CItemObjet* pItem)
{
	return S_OK;
}

HRESULT CPlayer::Gain_Item(ITEM_DATA* pItem, _uint iCount)
{
	return 	m_pInventory->Insert_Item(pItem, iCount);
}





void CPlayer::Set_Battle(bool bBattle)
{
	m_fBattleTime = 0.f;
	m_bBattle = bBattle;
	if(nullptr != m_pEquipModels[(_uint)EQUIP_ITEM_TYPE::WEAPON])
		static_cast<CWeapon*>(m_pEquipModels[(_uint)EQUIP_ITEM_TYPE::WEAPON])->Set_Battle(bBattle);
}

HRESULT CPlayer::Equip(EQUIP_ITEM_DATA* pItem)
{
	assert(pItem != nullptr);

	EQUIP_ITEM_TYPE eType = pItem->eEquipType;
	Remove_Child(m_pEquipModels[(_uint)eType]);
	Safe_Release(m_pEquipModels[(_uint)eType]);
	m_pEquipModels[(_uint)eType] = nullptr;

	//몸통 메쉬 켜고 끄기
	Set_BodyMeshActive(eType, false);
	
	CModelObject* pModel = nullptr;
	switch (eType)
	{
		//Socket
	case EQUIP_ITEM_TYPE::EAR:
	case EQUIP_ITEM_TYPE::HAT:
	{
		CBoneModelObject::BONEMODELOBJ_DESC tBoneModelDesc = {};
		tBoneModelDesc.eModelProtoLevelID = LEVEL_LOADING;
		strcpy_s(tBoneModelDesc.strModelProtoName, pItem->strModelTag);
		tBoneModelDesc.pSocketMatrix = (m_pBody)->Get_BoneMatrix("Bip01 Head");//Bip01 HeadNub_end
		pModel = static_cast<CBoneModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CBoneModelObject::m_szProtoTag, &tBoneModelDesc));
		if (nullptr == pModel) return E_FAIL;
		pModel->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));


		break;
	}		
	case EQUIP_ITEM_TYPE::CAPE:
	{
		CBoneModelObject::BONEMODELOBJ_DESC tBoneModelDesc = {};
		tBoneModelDesc.eModelProtoLevelID = LEVEL_LOADING;
		strcpy_s(tBoneModelDesc.strModelProtoName, pItem->strModelTag);
		tBoneModelDesc.pSocketMatrix = (m_pBody)->Get_BoneMatrix("Bip01 Spine1");
		pModel = static_cast<CBoneModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CBoneModelObject::m_szProtoTag, &tBoneModelDesc));
		if (nullptr == pModel) return E_FAIL;
		pModel->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));
		pModel->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(0, 0.07, 0, 1));
		pModel->Set_Animation(0);
		pModel->Set_AnimationLoop(0, true);

		break;
	}
		//Socket X2
	case EQUIP_ITEM_TYPE::WEAPON:
	{
		CWeapon::WEAPON_DESC WeaponDesc{};
		WeaponDesc.eModelProtoLevelID = LEVEL_LOADING;
		strcpy_s(WeaponDesc.strModelProtoName, pItem->strModelTag);
		WeaponDesc.pSocketMatrix = m_pBody->Get_BoneMatrix("Weapon_Hand_R_Point");
		WeaponDesc.pBackSocketMatrix = m_pBody->Get_BoneMatrix("Weapon_Back_B_Point");
		pModel = static_cast<CWeapon*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CWeapon::m_szProtoTag, &WeaponDesc));
		if (nullptr == pModel) return E_FAIL;
		pModel->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));
		static_cast<CWeapon*>(pModel)->Set_Battle(m_bBattle);
		m_bWeapon =true;
		break;
	}
		//Target
	case EQUIP_ITEM_TYPE::BOTTOM:
	case EQUIP_ITEM_TYPE::TOP:
	case EQUIP_ITEM_TYPE::SUIT:
	case EQUIP_ITEM_TYPE::GLOVES:
	case EQUIP_ITEM_TYPE::SHOES:
	{
		
		CModelObject::MODELOBJ_DESC tModelDesc = {};
		tModelDesc.eModelProtoLevelID = LEVEL_LOADING;
		strcpy_s(tModelDesc.strModelProtoName, pItem->strModelTag);
		tModelDesc.pMimicTarget = m_pBody->Get_ModelCom();
		pModel = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
		if (nullptr == pModel) return E_FAIL;
		break;

	}
	
	case EQUIP_ITEM_TYPE::LAST:
	default:
		break;
	}

	if (nullptr == pModel) return E_FAIL;
	Add_Child(pModel);
	m_pEquipModels[(_uint)eType] = pModel;


}

HRESULT CPlayer::UnEquip(EQUIP_ITEM_TYPE eType)
{
	Remove_Child(m_pEquipModels[(_uint)eType]);
	Safe_Release(m_pEquipModels[(_uint)eType]);
	m_pEquipModels[(_uint)eType] = nullptr;

	Set_BodyMeshActive(eType, true);

	return S_OK;
}

HRESULT CPlayer::Set_Deco(DECO_ITEM_DESC* pItem)
{
	return S_OK;
}

HRESULT CPlayer::Set_Customize(CUSTOMIZE_DESC* pItem)
{
	return S_OK;

}

void CPlayer::Set_BodyMeshActive(EQUIP_ITEM_TYPE eType, bool bActive)
{
	CHumanModelObject* pHuman = static_cast<CHumanModelObject*>(m_pBody);
	switch (eType)
	{
	case EQUIP_ITEM_TYPE::WEAPON:
		m_bWeapon = bActive;
		break;
	case EQUIP_ITEM_TYPE::TOP:
		pHuman->Set_MeshActive(CHumanModelObject::MESH_PART_ID::TOP, bActive);
		break;
	case EQUIP_ITEM_TYPE::BOTTOM:
		pHuman->Set_MeshActive(CHumanModelObject::MESH_PART_ID::BOTTOM, bActive);
		break;
	case EQUIP_ITEM_TYPE::GLOVES:
		pHuman->Set_MeshActive(CHumanModelObject::MESH_PART_ID::GLOVE, bActive);
		break;
	case EQUIP_ITEM_TYPE::SHOES:
		pHuman->Set_MeshActive(CHumanModelObject::MESH_PART_ID::SHOES, bActive);
		break;
	case EQUIP_ITEM_TYPE::SUIT:
		if (bActive)
		{
			Set_BodyMeshActive(EQUIP_ITEM_TYPE::TOP, m_pEquipModels[(_uint)EQUIP_ITEM_TYPE::TOP] == nullptr);
			Set_BodyMeshActive(EQUIP_ITEM_TYPE::BOTTOM, m_pEquipModels[(_uint)EQUIP_ITEM_TYPE::BOTTOM] == nullptr);
		}
		else
		{
			Set_BodyMeshActive(EQUIP_ITEM_TYPE::TOP, false);
			Set_BodyMeshActive(EQUIP_ITEM_TYPE::BOTTOM, false);

		}
	case EQUIP_ITEM_TYPE::LAST:
	default:
		break;
	}
}





CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pFaceStateMachine);
	Safe_Release(m_pInteractable);
}

void CPlayer::On_HPZero()
{
	//데드 애님 재생
	//
	m_bHPZero = true;
}

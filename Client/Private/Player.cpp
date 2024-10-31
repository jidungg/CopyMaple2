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

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
	for (_uint i = 0; i < (_uint)SKILL_ID::LAST; i++)
	{
		m_pSkill[i] = nullptr;
	}
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

}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CCharacter(Prototype)
{

	Safe_AddRef(m_pAnimStateMachine);
	m_pBody = Prototype.m_pBody;
	Safe_AddRef(m_pBody);
	for (_uint i = 0; i < (_uint)SKILL_ID::LAST; i++)
	{
		m_pSkill[i] = Prototype.m_pSkill[i];
		Safe_AddRef(m_pSkill[i]);
	}
	//for (auto& pEquip : m_pEquipModels)
	//{
	//	pEquip = Prototype.m_pEquipModels[(_uint)(&pEquip - &m_pEquipModels[0])];
	//	Safe_AddRef(pEquip);
	//}
	//for (auto& pDeco : m_pDecoModels)
	//{
	//	pDeco = Prototype.m_pDecoModels[(_uint)(&pDeco - &m_pDecoModels[0])];
	//	Safe_AddRef(pDeco);
	//}
	//for (auto& pCustomize : m_pCustomizes)
	//{
	//	pCustomize = Prototype.m_pCustomizes[(_uint)(&pCustomize - &m_pCustomizes[0])];
	//	Safe_AddRef(pCustomize);
	//}
	m_fRunSpeed = Prototype.m_fRunSpeed;
	m_fJumpPower = Prototype.m_fJumpPower;
	m_fFloorHeight = Prototype.m_fFloorHeight;
	m_fHeight = Prototype.m_fHeight;
	m_fUpForce = Prototype.m_fUpForce;
	m_vMoveDir = Prototype.m_vMoveDir;
	m_iRandomCondition = Prototype.m_iRandomCondition;
	m_fIdleTime = Prototype.m_fIdleTime;
	m_bWalk = Prototype.m_bWalk;
	m_bMove = Prototype.m_bMove;
	m_bWeapon = Prototype.m_bWeapon;
	m_bBattle = Prototype.m_bBattle;
	m_fBattleTime = Prototype.m_fBattleTime;
	m_fBattleTimeMax = Prototype.m_fBattleTimeMax;
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
	desc->fSpeedPerSec = m_fRunSpeed;
	desc->fRotationPerSec = 5.f;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;
	if (FAILED(Ready_AnimStateMachine()))
		return E_FAIL;
	if (FAILED(Ready_FaceStateMachine()))
		return E_FAIL;
	if (FAILED(Ready_Skill()))
		return E_FAIL;

	ITEM_DESC* pItemDesc = ITEMDB->Get_Data(ITEM_TYPE::EQUIP, (_uint)EQUIP_ITEM_ID::BASIC_STAFF);
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
	m_pBody->Set_Animation((_uint)ANIM_STATE::IDLE);

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

	//AnimStateMachine
	m_pAnimStateMachine = static_cast<CStateMachine*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Prototype_GameObject_StateMachine")));
	Add_Component(m_pAnimStateMachine, TEXT("Com_StateMachine"));
	m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CPlayer::On_StateChange, this, placeholders::_1));
	m_pAnimStateMachine->Register_OnSubStateChangeCallBack(bind(&CPlayer::On_SubStateChange, this, placeholders::_1));

	CState* pState; CTransition* pTransition; Condition* pCondition;
	m_pBody->Set_AnimationLoop((_uint)ANIM_STATE::IDLE, true);
	m_pBody->Set_AnimationLoop((_uint)ANIM_STATE::ATTACK_IDLE, true);
	m_pBody->Set_AnimationLoop((_uint)ANIM_STATE::RUN, true);
	m_pBody->Set_AnimationLoop((_uint)ANIM_STATE::WALK, true);
	m_pBody->Set_AnimationLoop((_uint)ANIM_STATE::STAFF_RUN, true);
	m_pBody->Set_AnimationLoop((_uint)ANIM_STATE::STAFF_ATTACK_IDLE, true);
	m_pBody->Set_AnimPostDelayPercent((_uint)ANIM_STATE::ATTACK_01, 0.8f);
	m_pBody->Set_AnimPostDelayPercent((_uint)ANIM_STATE::JUMP_ATTACK, 1.0f);
	m_pBody->Set_AnimPostDelayPercent((_uint)ANIM_STATE::STAFF_ATTACK, 0.5f);
	m_pBody->Set_AnimPostDelayPercent((_uint)ANIM_STATE::STAFF_JUMP_ATTACK, 1.0f);
	m_pBody->Set_AnimPostDelayPercent((_uint)ANIM_STATE::MAGICCLAW, 0.5f);
	m_pBody->Set_AnimPostDelayPercent((_uint)ANIM_STATE::KINDLING_01, 0.6f);

	//CONDITION
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::ANIM_END_TRIGGER));
	m_pAnimStateMachine->Add_TriggerConditionVariable(_uint(ANIM_CONDITION::ATTACK_TRIGGER));

	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::RANDOM), &m_iRandomCondition);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::UPFORCE), &m_fUpForce);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::HEIGHT), &m_fHeight);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::IDLETIME), &m_fIdleTime);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::WALK), &m_bWalk);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::MOVE), &m_bMove);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::WEAPON), &m_bWeapon);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::BATTLE), &m_bBattle);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::POSTDELAY_END), &m_bPostDelayEnd);
	m_pAnimStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::SKILL_ID), &m_iSkillID);


	//STATE
	for (_uint i = 0; i < (_uint)ANIM_STATE::LAST; i++)
		m_pAnimStateMachine->Add_State(i);


#pragma region SubState
	//SUBSTATE
//IDLE 
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::IDLE, (_uint)ANIM_STATE::STAFF_ATTACK_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::BATTLE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::WEAPON, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::IDLE, (_uint)ANIM_STATE::ATTACK_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::BATTLE, CONDITION_TYPE::EQUAL, true);

	//RUN
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::RUN, (_uint)ANIM_STATE::STAFF_RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::BATTLE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::WEAPON, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::RUN, (_uint)ANIM_STATE::WALK);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::WALK, CONDITION_TYPE::EQUAL, true);

	//JUMP
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::JUMP_UP_A, (_uint)ANIM_STATE::STAFF_JUMP_UP_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::BATTLE, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::JUMP_UP_B, (_uint)ANIM_STATE::STAFF_JUMP_UP_B);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::BATTLE, CONDITION_TYPE::EQUAL, true);

	//ATTACK
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::ATTACK_01, (_uint)ANIM_STATE::JUMP_ATTACK);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::HEIGHT, CONDITION_TYPE::GREATER, m_fFloorHeight);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::BASIC_ATTACK);
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::ATTACK_01, (_uint)ANIM_STATE::STAFF_ATTACK);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::BASIC_ATTACK);
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::ATTACK_01, (_uint)ANIM_STATE::STAFF_JUMP_ATTACK);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::HEIGHT, CONDITION_TYPE::GREATER, m_fFloorHeight);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::BASIC_ATTACK);
	pTransition = m_pAnimStateMachine->Add_SubTransition((_uint)ANIM_STATE::ATTACK_01, (_uint)ANIM_STATE::MAGICCLAW);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::WEAPON, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::SKILL_ID, CONDITION_TYPE::EQUAL, (_int)SKILL_ID::MAGIC_CLAW);
#pragma endregion
#pragma region MainTransition

	//Transition
	//IDLE
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::IDLE, (_uint)ANIM_STATE::ATTACK_01);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ATTACK_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::IDLE, (_uint)ANIM_STATE::JUMP_UP_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::UPFORCE, CONDITION_TYPE::EQUAL_GREATER, m_fJumpPower - 1.f);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::IDLE, (_uint)ANIM_STATE::RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::MOVE, CONDITION_TYPE::EQUAL, true);

	//ATTACK
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::ATTACK_01, (_uint)ANIM_STATE::IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::ATTACK_01, (_uint)ANIM_STATE::JUMP_DOWN_A);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ANIM_END_TRIGGER);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::HEIGHT, CONDITION_TYPE::GREATER, m_fFloorHeight);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::ATTACK_01, (_uint)ANIM_STATE::RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::POSTDELAY_END, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::MOVE, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::HEIGHT, CONDITION_TYPE::EQUAL_LESS, m_fFloorHeight);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::ATTACK_01, (_uint)ANIM_STATE::ATTACK_01);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::POSTDELAY_END, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ATTACK_TRIGGER);

	//JUMP_UP
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::JUMP_UP_A, (_uint)ANIM_STATE::JUMP_DOWN_A);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::JUMP_UP_A, (_uint)ANIM_STATE::ATTACK_01);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ATTACK_TRIGGER);

	//JUMP_DOWN
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::JUMP_DOWN_A, (_uint)ANIM_STATE::JUMP_LAND);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::HEIGHT, CONDITION_TYPE::EQUAL_LESS, m_fFloorHeight);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::JUMP_DOWN_A, (_uint)ANIM_STATE::ATTACK_01);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ATTACK_TRIGGER);


	//JUMP_LAND
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::JUMP_LAND, (_uint)ANIM_STATE::IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ANIM_END_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::JUMP_LAND, (_uint)ANIM_STATE::ATTACK_01);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ATTACK_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::JUMP_LAND, (_uint)ANIM_STATE::JUMP_UP_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::UPFORCE, CONDITION_TYPE::EQUAL_GREATER, m_fJumpPower - 1.f);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::JUMP_LAND, (_uint)ANIM_STATE::RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::MOVE, CONDITION_TYPE::EQUAL, true);

	//RUN
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::RUN, (_uint)ANIM_STATE::IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::MOVE, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::RUN, (_uint)ANIM_STATE::ATTACK_01);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, (_uint)ANIM_CONDITION::ATTACK_TRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition((_uint)ANIM_STATE::RUN, (_uint)ANIM_STATE::JUMP_UP_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::UPFORCE, CONDITION_TYPE::EQUAL_GREATER, m_fJumpPower - 1.f);

#pragma endregion

	m_pAnimStateMachine->Set_CurrentState((_uint)ANIM_STATE::IDLE);
	return S_OK;
}

HRESULT CPlayer::Ready_FaceStateMachine()
{
	//AnimStateMachine
	m_pFaceStateMachine = static_cast<CStateMachine*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Prototype_GameObject_StateMachine")));
	Add_Component(m_pFaceStateMachine, TEXT("Com_FaceStateMachine"));
	m_pFaceStateMachine->Register_OnStateChangeCallBack(bind(&CPlayer::On_FaceStateChange, this, placeholders::_1));

	m_pFaceStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::BATTLE), &m_bBattle);
	m_pFaceStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::INTERACTION), &m_bInteraction);
	m_pFaceStateMachine->Add_ConditionVariable(_uint(ANIM_CONDITION::PAINTIME), &m_fPainTimeAcc);

	//STATE ADD
	for (_uint i = 0; i < (_uint)CFace::FACE_STATE::LAST; i++)
		m_pFaceStateMachine->Add_State(i);

	//IDLE
	CTransition* pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::IDLE, (_uint)CFace::FACE_STATE::ATTACK);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::BATTLE, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::IDLE, (_uint)CFace::FACE_STATE::FOCUS);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::INTERACTION, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::IDLE, (_uint)CFace::FACE_STATE::PAIN);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::PAINTIME, CONDITION_TYPE::EQUAL_LESS, m_fPainTime);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::PAINTIME, CONDITION_TYPE::GREATER, 0.0f);

	//ATTACK
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::ATTACK, (_uint)CFace::FACE_STATE::IDLE);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::BATTLE, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::ATTACK, (_uint)CFace::FACE_STATE::PAIN);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::PAINTIME, CONDITION_TYPE::EQUAL_LESS, m_fPainTime);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::PAINTIME, CONDITION_TYPE::GREATER, 0.0f);

	//PAIN
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::PAIN, (_uint)CFace::FACE_STATE::ATTACK);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::PAINTIME, CONDITION_TYPE::EQUAL_GREATER, m_fPainTime);

	//FOCUS
	pTransition = m_pFaceStateMachine->Add_Transition((_uint)CFace::FACE_STATE::FOCUS, (_uint)CFace::FACE_STATE::IDLE);
	m_pFaceStateMachine->Bind_Condition(pTransition, (_uint)ANIM_CONDITION::INTERACTION, CONDITION_TYPE::EQUAL, false);

	m_pFaceStateMachine->Set_CurrentState((_uint)CFace::FACE_STATE::IDLE);
	return S_OK;
}



HRESULT CPlayer::Ready_Skill()
{
	CSkillManager* pSkillManager = SKILLMAN;

	m_pSkill[(_uint)SKILL_ID::MAGIC_CLAW] =  CSkill::Create(pSkillManager->Get_SkillData(SKILL_ID::MAGIC_CLAW),this);
	return S_OK;
}



void CPlayer::Receive_KeyInput(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyState(KEY::I) == KEY_STATE::DOWN)
		m_pInventory->Toggle_UI();
	if (m_pGameInstance->GetKeyState(KEY::P) == KEY_STATE::DOWN)
	{
	}
	ANIM_STATE eCurrentState = ANIM_STATE(m_pAnimStateMachine->Get_CurrentState());


	if (m_pGameInstance->GetKeyState(KEY::LSHIFT) == KEY_STATE::DOWN)
		m_bWalk = true;
	else if (m_pGameInstance->GetKeyState(KEY::LSHIFT) == KEY_STATE::UP)
		m_bWalk = false;

	//이하 딜레이 끝나면 누를 수 있는 키들
	if (m_pBody->Is_AnimPostDelayEnd() == false)
		return;
	if (m_pGameInstance->GetKeyState(KEY::RIGHT) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_E);
	if (m_pGameInstance->GetKeyState(KEY::LEFT) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_W);
	if (m_pGameInstance->GetKeyState(KEY::UP) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_N);
	if (m_pGameInstance->GetKeyState(KEY::DOWN) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_S);

	if (m_pGameInstance->GetKeyState(KEY::X) == KEY_STATE::DOWN)
	{
		m_pAnimStateMachine->Trigger_ConditionVariable((_uint)ANIM_CONDITION::ATTACK_TRIGGER);
		m_iSkillID = (_int)SKILL_ID::BASIC_ATTACK;
		Set_Battle(true);
	}
	if (m_pGameInstance->GetKeyState(KEY::C) == KEY_STATE::DOWN)
		if (m_fUpForce == 0.f)
			m_fUpForce = m_fJumpPower;
}
void CPlayer::Priority_Update(_float fTimeDelta)
{
	m_iRandomCondition = rand() % 100;
	m_fUpForce -= fTimeDelta * 9.8f;
	m_fHeight += m_fUpForce * fTimeDelta;
	if (m_fHeight <= m_fFloorHeight)
	{
		m_fUpForce = 0;
		m_fHeight = m_fFloorHeight;
	}
	m_bMove = XMVector4Equal(m_vMoveDir, XMVectorSet(0, 0, 0, 0)) == false;
	
	if (m_pAnimStateMachine->Get_CurrentState() == (_uint)ANIM_STATE::IDLE)
		m_fIdleTime += fTimeDelta;
	if (m_bBattle)
	{
		m_fBattleTime += fTimeDelta;
		if (m_fBattleTime >= m_fBattleTimeMax)
		{
			Set_Battle(false);
		}
	}
	if (m_pBody->Is_AnimEnd())
		m_pAnimStateMachine->Trigger_ConditionVariable((_uint)ANIM_CONDITION::ANIM_END_TRIGGER);
	m_bPostDelayEnd = m_pBody->Is_AnimPostDelayEnd();
}
void CPlayer::Update(_float fTimeDelta)
{
	_float4 vPos;
	XMStoreFloat4(&vPos,m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	vPos.y = m_fHeight;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4( &vPos));
	//m_pBody->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

	if (m_bMove)
	{
		m_pTransformCom->Go_Direction(m_vMoveDir, fTimeDelta);
		//m_pBody->Get_Transform()->LookToward(XMVectorSetY(m_vMoveDir, 0));
		m_pTransformCom->LookToward(XMVectorSetY(m_vMoveDir, 0));
	}
	__super::Update(fTimeDelta);

}
void CPlayer::On_SubStateChange(_uint iSubState)
{
	m_pBody->Switch_Animation(iSubState);
}
void CPlayer::On_FaceStateChange(_uint iState)
{
	static_cast<CHumanModelObject*>(m_pBody)->Set_FaceState((CFace::FACE_STATE)iState);
}
void CPlayer::Use_Skill(CSkill* pSkill)
{
	if (pSkill == nullptr)
		return;
	Set_Battle(true);
	m_pAnimStateMachine->Trigger_ConditionVariable((_uint)ANIM_CONDITION::ATTACK_TRIGGER);
	m_iSkillID = (_int)pSkill->Get_SkillDesc()->eID;
}

HRESULT CPlayer::Gain_Item(CItemObjet* pItem)
{
	return S_OK;
}

HRESULT CPlayer::Gain_Item(ITEM_DESC* pItem, _uint iCount)
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

HRESULT CPlayer::Equip(EQUIP_ITEM_DESC* pItem)
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



void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND,this);
	__super::Late_Update(fTimeDelta);
	if(m_fHeight <= m_fFloorHeight)
		m_vMoveDir = XMVectorSet(0, 0, 0, 0);
}

HRESULT CPlayer::Render()
{
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active() && child->Is_Dead() == false)
			child->Render();
	}
	return S_OK;
}


void CPlayer::On_StateChange(_uint iState)
{
	if (iState == (_uint)ANIM_STATE::IDLE)
	{
		m_fIdleTime = 0.f;
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
	for (auto& skill : m_pSkill)
		Safe_Release(skill);
}

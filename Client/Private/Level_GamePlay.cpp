#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Trace.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "UIQuickSlot.h"
#include "SkillManager.h"
#include "LevelChangeEvent.h"
#include "Monster.h"
#include "Bayar.h"
#include "Player.h"
#include "ModelObject.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	m_iLevelID = LEVEL_GAMEPLAY;
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;
	m_pPlayer = static_cast<CPlayer*>( m_pGameInstance->Get_FirstGameObject(LEVEL_GAMEPLAY, LAYER_PLAYER));
	assert(nullptr != m_pPlayer);
	m_pGameInstance->Possess(m_pPlayer);
	m_pPlayer->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(0,1,0,1));
	if (FAILED(Ready_Layer_BackGround(LAYER_TERRAIN)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_UI)))
		return E_FAIL;
	if (FAILED(Ready_Monster(LAYER_MONSTER)))
		return E_FAIL;

	CModelObject::MODELOBJ_DESC tBulletDesc;
	tBulletDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tBulletDesc.strModelProtoName, "eff_wizard_magicclaw_remain_01_a.model");
	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Lights()
{


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(LAYERID eLayerID)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Henesys"), LEVEL_GAMEPLAY, eLayerID, (void*)nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(LAYERID eLayerID)
{
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Monster(LAYERID eLayerID)
{
	CMonster::MONSTER_DESC tMonDesc;
	CMonster* pMonster;
	//tMonDesc.eMonID = MONSTER_ID::BAYAR;
	//tMonDesc.vHomePos = { 10,1,10,1 };
	//pMonster = static_cast<CMonster*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Bayar"),&tMonDesc));
	//m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY,LAYER_MONSTER, pMonster);
	//
	//tMonDesc.eMonID = MONSTER_ID::SNAIL;
	//tMonDesc.vHomePos = { 3,1,10,1 };
	//pMonster = static_cast<CMonster*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CMonster::m_szProtoTag, &tMonDesc));
	//m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, LAYER_MONSTER, pMonster);

	//tMonDesc.eMonID = MONSTER_ID::CRAB;
	//tMonDesc.vHomePos = { 3,1,13,1 };
	//pMonster = static_cast<CMonster*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CMonster::m_szProtoTag, &tMonDesc));
	//m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, LAYER_MONSTER, pMonster);

	return S_OK;

}
void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyState(KEY::T) == KEY_STATE::UP)
	{
		m_pGameInstance->Push_Event(CLevelChangeEvent::Create(CLevel_Loading::Create(m_pDevice,m_pContext, LEVEL_HOME)));
	}

}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("헤네시스"));
#endif

	return S_OK;
}

void CLevel_GamePlay::On_Start()
{
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_PLAYER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_INTERACTION, true);

}


CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}

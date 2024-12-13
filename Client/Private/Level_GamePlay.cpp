#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Trace.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "SkillManager.h"
#include "LevelChangeEvent.h"
#include "Monster.h"
#include "Bayar.h"
#include "Player.h"
#include "ModelObject.h"
#include "CubeTerrain.h"
#include "Client_Utility.h"
#include "BackGround.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_GamePlay::Initialize(void* pArg)
{
	LevelDesc* pDesc = static_cast<LevelDesc*>( pArg);
	//TERRAIN
	m_pCubeTerrain = static_cast<CCubeTerrain*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, m_iLevelID,
		pDesc->szCubeTerrainTag, nullptr));
	if (nullptr == m_pCubeTerrain)
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_iLevelID, LAYER_TERRAIN, m_pCubeTerrain)))
		return E_FAIL;

	//PLAYER
	m_pPlayer = static_cast<CPlayer*>( m_pGameInstance->Get_FirstGameObject(Get_CurrentTrueLevel(), LAYER_PLAYER));
	assert(nullptr != m_pPlayer);
	m_pGameInstance->Possess(m_pPlayer);
	m_pPlayer->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(0,1,0,1));
	auto pPortal = m_pCubeTerrain->Get_Portal((LEVELID)pDesc->iPrevLevelID);
	if (nullptr != pPortal)
		m_pPlayer->Set_Position(pPortal->Get_TransformPosition());
	else
	{
		auto pSpawn = m_pCubeTerrain->Get_PlayerSpawn();
		if (nullptr != pSpawn)
			m_pPlayer->Set_Position(pSpawn->Get_TransformPosition());
		else
			m_pPlayer->Set_Position(XMVectorSet(0, 1, 0, 1));
	}


	CBackGround::BACKGROUND_DESC tBGDesc{};
	tBGDesc.pPlayer = m_pPlayer;
	tBGDesc.pTerrain = m_pCubeTerrain;
	tBGDesc.szBackGroundImgTag = pDesc->szBackGroundImgTag;
	CBackGround* pBackGround = static_cast<CBackGround*>(m_pGameInstance->Clone_Proto_Object_Stock(CBackGround::m_szProtoTag, &tBGDesc));
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_NONCOLLISION, pBackGround);

return S_OK;
}

void CLevel_GamePlay::On_Start(_uint iPrevLevelID)
{

}


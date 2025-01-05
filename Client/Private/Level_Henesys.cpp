#include "stdafx.h"
#include "Level_Henesys.h"
#include "GameInstance.h"
#include "AttachableBodyPart.h"
#include "Client_Utility.h"
#include "Collider_Sphere.h"
#include "UIBundle.h"

CLevel_Henesys::CLevel_Henesys(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel_GamePlay(pDevice, pContext)
{
	m_iLevelID = LEVEL_HENESYS;
}

void CLevel_Henesys::On_Start(_uint iPrevLevelID)
{
	__super::On_Start(iPrevLevelID);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_PLAYER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_INTERACTION, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_PLAYER, true);

	m_pGameInstance->Start_BGM(LEVEL_HENESYS,TEXT("BGM_Henesys_01"));
	m_pGameInstance->Set_BGMVolume(100);
	UIBUNDLE->Set_HUDActive(true);
}

CLevel_Henesys* CLevel_Henesys::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CLevel_Henesys* pInstance = new CLevel_Henesys(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CLevel_Henesys");
		Safe_Release(pInstance);
	}

	return pInstance;
}

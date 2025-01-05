#include "stdafx.h"
#include "Level_HuntingPlace2.h"
#include "GameInstance.h"

CLevel_HuntingPlace2::CLevel_HuntingPlace2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel_GamePlay(pDevice, pContext)
{
	m_iLevelID = LEVEL_HUNTINGPLACE2;
}

void CLevel_HuntingPlace2::On_Start(_uint iPrevLevelID)
{
	__super::On_Start(iPrevLevelID);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_PLAYER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_INTERACTION, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_PLAYER, true);

	m_pGameInstance->Start_BGM(LEVEL_HUNTINGPLACE2, TEXT("BGM_Henesys_field_01"));

}

CLevel_HuntingPlace2* CLevel_HuntingPlace2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CLevel_HuntingPlace2* pInstance = new CLevel_HuntingPlace2(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CLevel_HuntingPlace2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

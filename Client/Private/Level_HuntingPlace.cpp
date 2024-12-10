#include "stdafx.h"
#include "Level_HuntingPlace.h"
#include "GameInstance.h"
#include "AttachableBodyPart.h"
#include "Client_Utility.h"
#include "Collider_Sphere.h"

CLevel_HuntingPlace::CLevel_HuntingPlace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel_GamePlay(pDevice, pContext)
{
	m_iLevelID = LEVEL_HUNTINGPLACE;
}

void CLevel_HuntingPlace::On_Start(_uint iPrevLevelID)
{
	__super::On_Start(iPrevLevelID);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_PLAYER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_INTERACTION, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_PLAYER, true);

}

CLevel_HuntingPlace* CLevel_HuntingPlace::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CLevel_HuntingPlace* pInstance = new CLevel_HuntingPlace(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CHuntingPlace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

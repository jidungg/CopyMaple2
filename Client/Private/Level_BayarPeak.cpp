#include "stdafx.h"
#include "Level_BayarPeak.h"
#include "GameInstance.h"
#include "AttachableBodyPart.h"
#include "Client_Utility.h"
#include "Collider_Sphere.h"

CLevel_BayarPeak::CLevel_BayarPeak(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel_GamePlay(pDevice, pContext)
{
	m_iLevelID = LEVEL_BAYARPEAK;
}


void CLevel_BayarPeak::On_Start(_uint iPrevLevelID)
{
	__super::On_Start(iPrevLevelID);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_PLAYER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_INTERACTION, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_PLAYER, true);

	m_pGameInstance->Start_BGM(LEVEL_BAYARPEAK, TEXT("BGM_Boss_01_Intro"), false);

}

void CLevel_BayarPeak::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bBGMIntroEnd == false)
	{
		if (false == m_pGameInstance->Is_BGMPlaying(LEVEL_BAYARPEAK, TEXT("BGM_Boss_01_Intro")))
		{
			m_pGameInstance->Start_BGM(LEVEL_BAYARPEAK, TEXT("BGM_Boss_01_Loop"), true);
			m_bBGMIntroEnd = true;
		}
	}
}

CLevel_BayarPeak* CLevel_BayarPeak::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CLevel_BayarPeak* pInstance = new CLevel_BayarPeak(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CLevel_BayarPeak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

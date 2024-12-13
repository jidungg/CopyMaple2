#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "Loader.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Henesys.h"
#include "Level_Home.h"
#include "Level_BayarPeak.h"
#include "Level_HuntingPlace.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	m_iLevelID = LEVEL_LOADING;
}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID)
{
	m_ePrevLevelID = (LEVELID) m_pGameInstance->Get_CurrentLevelID();
	m_eNextLevelID = eNextLevelID;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;
	
	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished())
	{
		
		CLevel*			pNewLevel = { nullptr };
		if(m_ePrevLevelID != -1)
		{
			m_pGameInstance->Move_DontDestroyObjects(m_ePrevLevelID, m_eNextLevelID);
			m_pGameInstance->Clear(m_ePrevLevelID);
		}

		CLevel_GamePlay::LEVEL_DESC tDesc{};
		tDesc.iPrevLevelID = m_ePrevLevelID;
		switch (m_eNextLevelID)
		{
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_HENESYS:
			tDesc.szCubeTerrainTag = TEXT("Prototype_GameObject_Henesys");
			tDesc.szBackGroundImgTag = TEXT("bg_henesys_a.dds");
			pNewLevel = CLevel_Henesys::Create(m_pDevice, m_pContext,&tDesc);
			break;
		case LEVEL_BAYARPEAK:
			tDesc.szCubeTerrainTag = TEXT("Prototype_GameObject_BayarPeak");
			tDesc.szBackGroundImgTag = TEXT("bg_perion_c.dds");
			pNewLevel = CLevel_BayarPeak::Create(m_pDevice, m_pContext, &tDesc);
			break;
		case LEVEL_HOME:
			tDesc.szCubeTerrainTag = TEXT("Prototype_GameObject_MyHome");
			tDesc.szBackGroundImgTag = TEXT("bg_indoor_a.dds");
			pNewLevel = CLevel_Home::Create(m_pDevice, m_pContext, &tDesc);
			break;
		case LEVEL_HUNTINGPLACE:
			tDesc.szCubeTerrainTag = TEXT("Prototype_GameObject_HuntingPlace");
			tDesc.szBackGroundImgTag = TEXT("bg_henesys_b.dds");
			pNewLevel = CLevel_HuntingPlace::Create(m_pDevice, m_pContext, &tDesc);
			break;
		}

		if (nullptr == pNewLevel)
			return;

		if (FAILED(m_pGameInstance->Open_Level(m_eNextLevelID, pNewLevel)))
			return;
	}
}

HRESULT CLevel_Loading::Render()
{

#ifdef _DEBUG
	m_pLoader->Show_Debug();
#endif

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}

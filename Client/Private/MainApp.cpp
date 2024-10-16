#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"

#include "Level_Loading.h"
#include "Loader.h"
#include "ItemDataBase.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);

	/*D3D11_SAMPLER_DESC*/
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hInstance =  g_hInst;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.isWindowed = true;
	EngineDesc.iNumLevels = LEVEL_END;
	EngineDesc.iViewportWidth = g_iWinSizeX;
	EngineDesc.iViewportHeight = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;
	XMMATRIX matGlobal = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	matGlobal = matGlobal * XMMatrixRotationY(XMConvertToRadians(180.f));
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_GLOBAL, matGlobal);
	if (FAILED(SetUp_StartLevel(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	if (nullptr != m_pGameInstance)
		m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Render_Begin()))
		return E_FAIL;

	m_pGameInstance->Draw();

	if (FAILED(m_pGameInstance->Render_End()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_StartLevel(LEVELID eLevelID)
{
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING,
		CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;
	
	return S_OK;
}

CMainApp * CMainApp::Create()
{	
	CMainApp*	pInstance = new CMainApp();	

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{

	/* �θ� ����� �����Ѵ�. */
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	CItemDataBase::DestroyInstance();
	Safe_Release(m_pGameInstance);
	CGameInstance::Release_Engine();

}


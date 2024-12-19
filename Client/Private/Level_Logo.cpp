#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "Inventory.h"
#include "PlayerInfo.h"
#include "Camera_Trace.h"
#include "UIBundle.h"
#include "Player.h"
#include "EffectManager.h"
#include "ItemDatabase.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
		m_iLevelID = { LEVEL_LOGO };
}
HRESULT CLevel_Logo::Initialize(void* pArg)
{
	if (FAILED(Ready_Layer_BackGround(LAYER_UI)))
		return E_FAIL;
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTOINAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 0.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;
	if (FAILED(ITEMDB->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;
	if (FAILED(Ready_UI()))
		return E_FAIL;
	if (FAILED(INVENTORY->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(PLAYERINIFO->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;
	if (FAILED(Ready_Camera(LAYERID::LAYER_CAMERA)))
		return E_FAIL;
	if (FAILED(EFFECT_MANAGER->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	UIBUNDLE->Initialize_PlayerInfo(PLAYERINIFO->Get_Player());
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Camera(LAYERID eLayerID)
{

	CCamera_Trace::TRACECAMERA_DESC		CamDesc{};

	CamDesc.fSpeedPerSec = 5.f;
	CamDesc.fRotationPerSec = XMConvertToRadians(90.f);
	CamDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CamDesc.fFovY = XMConvertToRadians(60.f);
	CamDesc.fNear = 0.1f;
	CamDesc.fFar = 1000.f;
	CamDesc.vEye = _float3(0.f, 3.f, 3.f);
	CamDesc.vAt = _float3(0.f, 0.f, 0.f);
	CamDesc.vFarArm = _vector{ -3.f, 5.f, -3.f, 1.f };
	CamDesc.vCloseArm = _vector{ -1.5f, 0.5f, -1.5f, 1.f };
	CamDesc.pTarget = static_cast<CPlayer*>(m_pGameInstance->Get_FirstGameObject(LEVEL_LOGO, LAYER_PLAYER));;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_Camera_Trace"),
		LEVEL_LOGO, eLayerID, &CamDesc,true)))
		return E_FAIL;
	return S_OK;
}
void CLevel_Logo::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_HENESYS));
	}

}

HRESULT CLevel_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고."));
#endif

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(LAYERID eID)
{
	CBackGround::BACKGROUND_DESC		Desc{};
	Desc.fXOffset = 0;
	Desc.fYOffset = 0;
	Desc.fSizeX = g_iWinSizeX;
	Desc.fSizeY = g_iWinSizeY;
	Desc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT,LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"), nullptr));


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, CUIPanel::m_szProtoTag,
		LEVEL_LOGO, eID, &Desc,false)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_UI()
{
	CUIBundle::UIBUNDLE_DESC tDesc{};
	tDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tDesc.ePivotType = CORNOR_TYPE::CENTER;
	tDesc.fSizeX = g_iWinSizeX;
	tDesc.fSizeY = g_iWinSizeY;
	tDesc.fXOffset = 0;
	tDesc.fYOffset = 0;
	tDesc.pDevice = m_pDevice;
	tDesc.pContext = m_pContext;
	UIBUNDLE->Initialize(&tDesc);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_UI,UIBUNDLE,true)))
		return E_FAIL;
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}

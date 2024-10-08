#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Player.h"
#include "Camera_Trace.h"
#include "Camera_Free.h"
#include "Level_Loading.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	m_iLevelID = LEVEL_GAMEPLAY;
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTOINAL;
	LightDesc.vDirection = _float4(-1.f, -1.f, -1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, strLayerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC		PlayerDesc{};

	PlayerDesc.fSpeedPerSec = 5.f;
	PlayerDesc.fRotationPerSec = XMConvertToRadians(90.f);

	CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, TEXT("Prototype_GameObject_Player"), &PlayerDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, strLayerTag, pPlayer)))
		return E_FAIL;

	CCamera_Trace::TRACECAMERA_DESC		CamDesc{};

	CamDesc.fSpeedPerSec = 5.f;
	CamDesc.fRotationPerSec = XMConvertToRadians(90.f);
	CamDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CamDesc.fFovY = XMConvertToRadians(60.f);
	CamDesc.fNear = 0.1f;
	CamDesc.fFar = 1000.f;
	CamDesc.vEye = _float3(0.f, 3.f, -3.f);
	CamDesc.vAt = _float3(0.f, 0.f, 0.f);
	CamDesc.vArm = _float3(0.f, 10.f, -7.f);
	CamDesc.pTarget = pPlayer;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_Camera_Trace"),
		LEVEL_GAMEPLAY, strLayerTag, &CamDesc)))
		return E_FAIL;

	return S_OK;
}
void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyState(KEY::T) == KEY_STATE::UP)
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_HOME));
}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
#endif

	return S_OK;
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

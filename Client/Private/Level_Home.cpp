#include "stdafx.h"
#include "Level_Home.h"

#include "UIPanel.h"
#include "GameInstance.h"
#include "Player.h"
#include "Camera_Trace.h"
#include "UIHomeDialog.h"
#include "Physics.h"
#include "Builder.h"
#include "CubeTerrain.h"
#include "Camera_Trace.h"
#include "Player.h"
#include "Collider.h"
#include "TerrainObject.h"
#include "ItemDataBase.h"


CLevel_Home::CLevel_Home(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
	m_iLevelID = LEVEL_HOME;

}


HRESULT CLevel_Home::Initialize()
{
	//if(FAILED(Ready_Lights()))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	CBuilder::BUILDER_DESC BuilderDesc{};
	BuilderDesc.fRotationPerSec = 10;
	BuilderDesc.fSpeedPerSec = 4;
	BuilderDesc.pCubeTerrain = m_pCubeTerrain;
	m_pBuilder = static_cast<CBuilder*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_HOME, CBuilder::m_szProtoTag, &BuilderDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, TEXT("Layer_Builder"), m_pBuilder)))
		return E_FAIL;
	m_pBuilder->Set_Active(false);

	m_pItemData = ITEMDB->GetItemMap(ITEM_TYPE::BUILD);
	m_pItemIter = m_pItemData->begin();
	m_pBuilder->Set_BuildItem(m_pItemIter->second);

return S_OK;
}

void CLevel_Home::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_pGameInstance->GetKeyState(KEY::B) == KEY_STATE::DOWN)
	{
		m_bBuildMode = !m_bBuildMode;
		m_pBuilder->Set_Active(m_bBuildMode);
		if(m_bBuildMode)
		{
			m_pCamera->Set_Target(m_pBuilder);
			m_pGameInstance->Possess(m_pBuilder);
			XMVECTOR vPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			_float4 fPos;
			XMStoreFloat4(&fPos, vPos);
			m_pBuilder->Set_Position(XMLoadFloat4(&m_pCubeTerrain->IndexToPos(m_pCubeTerrain->PosToIndex(fPos))));
		}
		else
		{
			m_pCamera->Set_Target(m_pPlayer);
			m_pGameInstance->Possess(m_pPlayer);
		}
	}
	if (m_bBuildMode)
	{
		//터레인 피킹해서 해당 위치로 이동
		if (m_pGameInstance->GetMouseKeyState(MOUSE_KEY::LB) == KEY_STATE::UP)
		{
			POINT tPosition = m_pGameInstance->Get_MousePos();
			Ray tRay{ tPosition ,g_iWinSizeX, g_iWinSizeY,
				m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW),
				m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ) };

			RAYCASTHIT tHitInfo;
			bool bHit = m_pGameInstance->RayCast(tRay, &tHitInfo);
			if (bHit)
			{
				CTerrainObject* pObj = dynamic_cast<CTerrainObject*>( tHitInfo.pCollider->Get_Owner());
				if (pObj == nullptr)
					return;
				_uint iIndex = pObj->Get_Index();
				XMUINT3 iSize = m_pCubeTerrain->Get_Size();
				iIndex += iSize.x * iSize.z;

				m_pBuilder->Set_Position(XMLoadFloat4( &m_pCubeTerrain->IndexToPos(iIndex)));
			}

		}
		//다음 아이템 선택
		if (m_pGameInstance->GetKeyState(KEY::F) == KEY_STATE::DOWN)
		{
			m_pItemIter++;
			if (m_pItemIter == m_pItemData->end())
				m_pItemIter = m_pItemData->begin();
			m_pBuilder->Set_BuildItem((*m_pItemIter).second);
	
		}
		//맵 저장
		if (m_pGameInstance->GetKeyState(KEY::H) == KEY_STATE::DOWN)
		{
			m_pCubeTerrain->Save_To_Json("../Bin/Resources/Json/MyHome.json");
		}
	}
	else
	{

	}

}

HRESULT CLevel_Home::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("집."));
#endif

	return S_OK;
}

HRESULT CLevel_Home::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	m_pCubeTerrain = static_cast<CCubeTerrain*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_HOME, 
		TEXT("Prototype_GameObject_MyHome"), nullptr));
	if (nullptr == m_pCubeTerrain)
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, strLayerTag, m_pCubeTerrain)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Home::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUIHomeDialog::HOMEDIALOG_DESC PanelDesc{};
	PanelDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	PanelDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	PanelDesc.fXOffset = 0;
	PanelDesc.fYOffset = 0;
	PanelDesc.fSizeX = g_iWinSizeX/2;
	PanelDesc.fSizeY = g_iWinSizeY/4;
	PanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, m_iLevelID, TEXT("UI_Texture_HomeDialog"), nullptr));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, TEXT("Prototype_GameObject_HomeDialog"),
		LEVEL_HOME, strLayerTag, &PanelDesc)))
		return E_FAIL;

	return S_OK; 
}

HRESULT CLevel_Home::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC		PlayerDesc{};

	PlayerDesc.fSpeedPerSec = 5.f;
	PlayerDesc.fRotationPerSec = XMConvertToRadians(90.f);

	m_pPlayer = static_cast<CPlayer*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, TEXT("Prototype_GameObject_Player"), &PlayerDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, strLayerTag, m_pPlayer)))
		return E_FAIL;
	m_pGameInstance->Possess(m_pPlayer);
	m_pPlayer->Set_Position(XMLoadFloat4(&m_pCubeTerrain->IndexToPos(100)));

	CCamera_Trace::TRACECAMERA_DESC		CamDesc{};

	CamDesc.fSpeedPerSec = 5.f;
	CamDesc.fRotationPerSec = XMConvertToRadians(90.f);
	CamDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CamDesc.fFovY = XMConvertToRadians(60.f);
	CamDesc.fNear = 0.1f;
	CamDesc.fFar = 1000.f;
	CamDesc.vEye = _float3(0.f, 3.f, -3.f);
	CamDesc.vAt = _float3(0.f, 0.f, 0.f);
	CamDesc.vArm = _float3(-7.f, 10.f, -7.f);
	CamDesc.pTarget = m_pPlayer;

	m_pCamera = static_cast<CCamera_Trace*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, TEXT("Prototype_GameObject_Camera_Trace"), &CamDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, strLayerTag, m_pCamera)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Home::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTOINAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Home* CLevel_Home::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Home* pInstance = new CLevel_Home(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Home");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Home::Free()
{
	__super::Free();
}

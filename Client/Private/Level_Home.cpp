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
	m_pItemData = ITEMDB->GetItemMap(ITEM_TYPE::BUILD);
	m_pItemIter = m_pItemData->begin();

	if (FAILED(Ready_Layer_BackGround(LAYER_TERRAIN)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_CAMERA)))
		return E_FAIL;
	
	CBuilder::BUILDER_DESC BuilderDesc{};
	BuilderDesc.fRotationPerSec = 10;
	BuilderDesc.fSpeedPerSec = 4;
	BuilderDesc.pCubeTerrain = m_pCubeTerrain;
	m_pBuilder = static_cast<CBuilder*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_HOME, CBuilder::m_szProtoTag, &BuilderDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, LAYER_PLAYER, m_pBuilder)))
		return E_FAIL;
	m_pBuilder->Set_Active(false);


	Set_BuildItem((BUILD_ITEM_ID)static_cast<BUILD_ITEM_DATA*>( m_pItemIter->second)->iItemID);

	
return S_OK;
}

void CLevel_Home::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_pGameInstance->GetKeyState(KEY::B) == KEY_STATE::DOWN)
	{
		m_bBuildMode = !m_bBuildMode;
		m_pBuilder->Set_Active(m_bBuildMode);
		m_pHomeDialog->Set_Active(m_bBuildMode);
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

			m_pBuilder->Set_BuildItem((BUILD_ITEM_ID)static_cast<BUILD_ITEM_DATA*>((*m_pItemIter).second)->iItemID);
	
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

void CLevel_Home::On_Start()
{
	Ready_Layer_UI(LAYER_UI);

	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_PLAYER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_INTERACTION, true);
}


void CLevel_Home::On_BuildItemSelected(void* pArg)
{
	const BUILD_ITEM_DATA* pDesc =static_cast<const BUILD_ITEM_DATA*>( reinterpret_cast<CUIItemIndicator*>(pArg)->Get_ItemDesc());
	Set_BuildItem((BUILD_ITEM_ID)(pDesc)->iItemID);
}

void CLevel_Home::Set_BuildItem(BUILD_ITEM_ID eID)
{
	m_pBuilder->Set_BuildItem(eID);
}

HRESULT CLevel_Home::Ready_Layer_BackGround(LAYERID eLayerId)
{
	m_pCubeTerrain = static_cast<CCubeTerrain*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_HOME, 
		TEXT("Prototype_GameObject_MyHome"), nullptr));
	if (nullptr == m_pCubeTerrain)
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, eLayerId, m_pCubeTerrain)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Home::Ready_Layer_UI(LAYERID eLayerId)
{
	CUIHomeDialog::HOMEDIALOG_DESC PanelDesc{};
	PanelDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	PanelDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	PanelDesc.fXOffset = 0;
	PanelDesc.fYOffset = 0;
	PanelDesc.fSizeX = g_iWinSizeX/2;
	PanelDesc.fSizeY = g_iWinSizeY/4;
	PanelDesc.vBorder = {3,3,3,3 };
	PanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, m_iLevelID, TEXT("UI_Texture_HomeDialog"), nullptr));
	list<UIListItemData*> listData;
	for (auto& i : *m_pItemData)
		listData.push_back(i.second);
	PanelDesc.listData = &listData;

	m_pHomeDialog = static_cast<CUIHomeDialog*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_HOME, TEXT("Prototype_GameObject_HomeDialog"), &PanelDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVELID::LEVEL_HOME, eLayerId, m_pHomeDialog)))
		return E_FAIL;
	function<void(void*)> f = bind(&CLevel_Home::On_BuildItemSelected, this, placeholders::_1);
	m_pHomeDialog->Register_OnClickCallback(f);
	m_pHomeDialog->Set_Active(false);
	return S_OK; 
}

HRESULT CLevel_Home::Ready_Layer_Camera(LAYERID eLayerId)
{
	m_pPlayer  = static_cast<CPlayer*>(m_pGameInstance->Get_FirstGameObject(LEVEL_HOME, LAYER_PLAYER));;
	m_pCamera = static_cast<CCamera_Trace*>(m_pGameInstance->Get_FirstGameObject(LEVEL_HOME, LAYER_CAMERA));;
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

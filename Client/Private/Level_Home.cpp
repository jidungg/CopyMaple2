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

#include "EffectManager.h"
#include "EffModelObject.h"


CLevel_Home::CLevel_Home(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel_GamePlay(pDevice, pContext)
{
	m_iLevelID = LEVEL_HOME;

}


HRESULT CLevel_Home::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_pItemData = ITEMDB->GetItemMap(ITEM_TYPE::BUILD);
	m_pItemIter = m_pItemData->begin();

	//CAMERA
	m_pCamera = static_cast<CCamera_Trace*>(m_pGameInstance->Get_FirstGameObject(LEVEL_HOME, LAYER_CAMERA));;

	//BUILDER
	CBuilder::BUILDER_DESC BuilderDesc{};
	BuilderDesc.fRotationPerSec = 10;
	BuilderDesc.fSpeedPerSec = 4;
	BuilderDesc.pCubeTerrain = m_pCubeTerrain;
	m_pBuilder = static_cast<CBuilder*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_HOME, CBuilder::m_szProtoTag, &BuilderDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, LAYER_NONCOLLISION, m_pBuilder)))
		return E_FAIL;
	m_pBuilder->Set_Active(false);

	Set_BuildItem(static_cast<BUILD_ITEM_DATA*>( m_pItemIter->second)->iItemID);

	Ready_Layer_UI(LAYER_UI);

	CEffModelObject::EFFECTOBJ_DESC tEffDesc;
	tEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_02_c_b.effmodel");
	 m_pEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffDesc));
	m_pEffect->Set_Active(false);
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_HOME, LAYER_NONCOLLISION, m_pEffect)))
		return E_FAIL;
	//m_pEffect->Set_Transform(_vector{0,1,0}, _vector{0,0,0}, 1 / 150);
return S_OK;
}

void CLevel_Home::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	//TMP----------------------------------------------
	if (m_pGameInstance->GetKeyState(KEY::F1) == KEY_STATE::DOWN)
		m_pEffect->Start_Animation(0,false,-1,0.2);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pEffect);
	//----------------------------------------------
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
			m_pBuilder->Set_Position(m_pCubeTerrain->IndexToPos(m_pCubeTerrain->PosToIndex(fPos)));
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

				m_pBuilder->Set_Position( m_pCubeTerrain->IndexToPos(iIndex));
			}

		}
		//다음 아이템 선택
		if (m_pGameInstance->GetKeyState(KEY::F) == KEY_STATE::DOWN)
		{
			m_pItemIter++;
			if (m_pItemIter == m_pItemData->end())
				m_pItemIter = m_pItemData->begin();

			Set_BuildItem(m_pItemIter->first);
			m_pHomeDialog->Select_NextItem();
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

void CLevel_Home::On_Start(_uint iPrevLevelID)
{
	__super::On_Start(iPrevLevelID);


	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_PLAYER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_INTERACTION, true);

	
}


void CLevel_Home::On_UIBuildItemSelected(void* pArg)
{
	CUIButtonItemIndicator* pIndicator = reinterpret_cast<CUIButtonItemIndicator*>(pArg);
	pIndicator->Get_ListItemIndex();
	const BUILD_ITEM_DATA* pDesc = reinterpret_cast<const BUILD_ITEM_DATA*>(pIndicator->Get_ItemDesc());
	Set_BuildItem((pDesc)->iItemID);
}

void CLevel_Home::Set_BuildItem(_uint iID)
{
	m_pBuilder->Set_BuildItem(iID);
	m_pItemIter = m_pItemData->find(iID);
}


HRESULT CLevel_Home::Ready_Layer_UI(LAYERID eLayerId)
{
	CUIHomeDialog::HOMEDIALOG_DESC homDialogDesc{};
	homDialogDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	homDialogDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	homDialogDesc.fXOffset = 0;
	homDialogDesc.fYOffset = 0;
	//homDialogDesc.fSizeX = 594;
	//homDialogDesc.fSizeY = 180;
	homDialogDesc.vBorder = {3,3,3,3 };
	homDialogDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, m_iLevelID, TEXT("UI_Texture_HomeDialog"), nullptr));
	list<UIListItemData*> listData;
	for (auto& i : *m_pItemData)
		listData.push_back(i.second);
	homDialogDesc.listData = &listData;

	m_pHomeDialog = static_cast<CUIHomeDialog*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_HOME, TEXT("Prototype_GameObject_HomeDialog"), &homDialogDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVELID::LEVEL_HOME, eLayerId, m_pHomeDialog)))
		return E_FAIL;
	function<void(void*)> f = bind(&CLevel_Home::On_UIBuildItemSelected, this, placeholders::_1);
	m_pHomeDialog->Register_OnClickCallback(f);
	m_pHomeDialog->Set_Active(false);
	return S_OK; 
}


CLevel_Home* CLevel_Home::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CLevel_Home* pInstance = new CLevel_Home(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
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

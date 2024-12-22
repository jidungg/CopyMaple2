#include "stdafx.h"
#include "UIPlayerInfo.h"
#include "PlayerInfo.h"
#include "GameInstance.h"
#include "UIPlayerInfoSlot.h"
#include "PlayerInfoSlot.h"


CUIPlayerInfo::CUIPlayerInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUICommonWindow(pDevice, pContext)
{
}

CUIPlayerInfo::CUIPlayerInfo(const CUIPlayerInfo& Prototype)
	: CUICommonWindow(Prototype)
{
}

HRESULT CUIPlayerInfo::Initialize(void* pArg)
{
	//239 + 422 + 
	UIPLAYERINFO_DESC* pDesc = static_cast<UIPLAYERINFO_DESC*>(pArg);
	pDesc->bDraggableX = true;
	pDesc->bDraggableY = true;
	pDesc->eAnchorType = CORNOR_TYPE::LEFT_TOP;
	pDesc->ePivotType = CORNOR_TYPE::LEFT_TOP;
	pDesc->fSizeX = m_fBackSize.x + m_fBackBorderSize.x *2 + m_fDashBoardSize.x + m_fCommonMargin.x *3;
	pDesc->fSizeY = m_fHeaderHeight + m_fDashBoardSize.y + m_fCommonMargin.y *2  ;
	pDesc->fXOffset = 100;
	pDesc->fYOffset = 100;
	pDesc->vBorder = { m_fHeaderHeight,m_fCommonMargin.y,m_fCommonMargin.x,m_fCommonMargin.x };
	pDesc->szIconProtoTag = TEXT("playerinfo_icon.dds");
	pDesc->szTitleText = TEXT("캐릭터 정보");
if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	m_pPlayerInfo = pDesc->pPlayerInfo;


	CUIPanel::PANEL_DESC tPanelDesc;
	tPanelDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tPanelDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tPanelDesc.fXOffset = m_fCommonMargin.x + (m_fBackBorderSize.x - 3);
	tPanelDesc.fYOffset = m_fHeaderHeight + m_fCommonMargin.y + (m_fDashBoardSize.y - m_fBackSize.y) / 2;
	tPanelDesc.fSizeX = m_fBackSize.x;
	tPanelDesc.fSizeY = m_fBackSize.y;
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("playerinfo_back.dds"), nullptr));
	tPanelDesc.vBorder = { m_fBackBorderSize.y,m_fBackBorderSize.y,m_fBackBorderSize.x,m_fBackBorderSize.x };
	m_pBackPanel = static_cast<CUIPanel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIPanel::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pBackPanel);

	tPanelDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tPanelDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tPanelDesc.fXOffset = -(m_fBackBorderSize.x - 3);
	tPanelDesc.fYOffset = -(m_fBackBorderSize.y - 3);
	tPanelDesc.fSizeX = m_fBackSize.x + (m_fBackBorderSize.x - 3) * 2;
	tPanelDesc.fSizeY = m_fDashBoardSize.y;
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("playerinfo_back_border.dds"), nullptr));
	tPanelDesc.vBorder = { m_fBackBorderSize.y,m_fBackBorderSize.y,m_fBackBorderSize.x,m_fBackBorderSize.x };
	m_pBackBorder = static_cast<CUIPanel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIPanel::m_szProtoTag, &tPanelDesc));
	m_pBackPanel->Add_Child(m_pBackBorder);


	tPanelDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tPanelDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	tPanelDesc.fXOffset = -m_fCommonMargin.x;
	tPanelDesc.fYOffset = m_fHeaderHeight + m_fCommonMargin.y;
	tPanelDesc.fSizeX = m_fDashBoardSize.x;
	tPanelDesc.fSizeY = m_fDashBoardSize.y;
	tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("playerinfo_dashboard.dds"), nullptr));
	tPanelDesc.vBorder = { m_fCommonMargin.y,m_fCommonMargin.y,m_fCommonMargin.x,m_fCommonMargin.x };
	m_pDashBoardBack = static_cast<CUIPanel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIPanel::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pDashBoardBack);

	if (FAILED(Ready_Slots()))
		return E_FAIL;

	
	return S_OK;
}
HRESULT CUIPlayerInfo::Ready_Slots()
{
	CUIPlayerInfoSlot::UIPLAYERINFO_SLOT_DESC tSlotDesc;
	tSlotDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tSlotDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tSlotDesc.fSizeX = m_fSlotSize.x;
	tSlotDesc.fSizeY = m_fSlotSize.y;
	tSlotDesc.pPlayerInfo = this;
	for (_uint i = 0; i < 6; i++)
	{
		tSlotDesc.fXOffset = m_fCommonMargin.x+ m_fInnerBackBorderSize.x;
		tSlotDesc.fYOffset = m_fCommonMargin.y + m_fInnerBackBorderSize .y+ (m_fSlotSize.y + m_fCommonMargin.y) * i;
		tSlotDesc.eSlotId = (SLOT_ID)i;
		m_arrSlot[i] = static_cast<CUIPlayerInfoSlot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIPlayerInfoSlot::m_szProtoTag, &tSlotDesc));
		m_pBackBorder->Add_Child(m_arrSlot[i]);
	}
	tSlotDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tSlotDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	for (_uint i = 6; i < SLOT_LAST; i++)
	{
		tSlotDesc.fXOffset = -(m_fInnerBackBorderSize.x+ m_fCommonMargin.x );
		tSlotDesc.fYOffset = m_fCommonMargin.y + m_fInnerBackBorderSize .y+ (m_fSlotSize.y + m_fCommonMargin.y) * (i - 6);
		tSlotDesc.eSlotId = (SLOT_ID)i;
		m_arrSlot[i] = static_cast<CUIPlayerInfoSlot*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIPlayerInfoSlot::m_szProtoTag, &tSlotDesc));
		m_pBackBorder->Add_Child(m_arrSlot[i]);
	}
	return S_OK;
}

void CUIPlayerInfo::Update_Slot(SLOT_ID eSlotID, CPlayerInfoSlot* pItemSlot)
{
	m_arrSlot[eSlotID]->Set_Item(pItemSlot->Get_ItemData());
}

void CUIPlayerInfo::Switch_Tab(TAB_ID eTab)
{
}

void CUIPlayerInfo::Select_Slot(SLOT_ID eSlotID)
{
}

list<CUIPlayerInfo::SLOT_ID> CUIPlayerInfo::Item_To_SlotID(const ITEM_DATA* pItemData)
{
	list<SLOT_ID> listSlotID;
	ITEM_TYPE eType = pItemData->eITemType;
	switch (eType)
	{
	case Client::ITEM_TYPE::EQUIP:
	{
		EQUIP_ITEM_TYPE eEquipType = static_cast<const EQUIP_ITEM_DATA*>(pItemData)->eEquipType;
		switch (eEquipType)
		{
		case Client::EQUIP_ITEM_TYPE::HAT:
			listSlotID.push_back(SLOT_ID::HAT);
			break;
		case Client::EQUIP_ITEM_TYPE::TOP:
			listSlotID.push_back(SLOT_ID::TOP);
			break;
		case Client::EQUIP_ITEM_TYPE::BOTTOM:
			listSlotID.push_back(SLOT_ID::BOTTOM);
			break;
		case Client::EQUIP_ITEM_TYPE::SUIT:
			listSlotID.push_back(SLOT_ID::TOP);
			listSlotID.push_back(SLOT_ID::BOTTOM);
			break;
		case Client::EQUIP_ITEM_TYPE::GLOVES:
			listSlotID.push_back(SLOT_ID::GLOVES);
			break;
		case Client::EQUIP_ITEM_TYPE::SHOES:
			listSlotID.push_back(SLOT_ID::SHOES);
			break;
		case Client::EQUIP_ITEM_TYPE::CAPE:
			listSlotID.push_back(SLOT_ID::CAPE);
			break;
		case Client::EQUIP_ITEM_TYPE::WEAPON:
			listSlotID.push_back(SLOT_ID::WEAPON);
			break;
		case Client::EQUIP_ITEM_TYPE::EAR:
			listSlotID.push_back(SLOT_ID::EAR);
			break;
		case Client::EQUIP_ITEM_TYPE::LAST:
		default:
			break;
		}
		break;
	}
	case Client::ITEM_TYPE::DECO:
	{
		DECO_ITEM_TYPE eDecoType = static_cast<const DECO_ITEM_DATA*>(pItemData)->eDecoType;
		switch (eDecoType)
		{
		case Client::DECO_ITEM_TYPE::FACE:
			listSlotID.push_back(SLOT_ID::FACE);
			break;
		case Client::DECO_ITEM_TYPE::LAST:
		default:
			break;
		}
		break;
	}
	case Client::ITEM_TYPE::CONSUMABLE:
	case Client::ITEM_TYPE::BUILD:
	case Client::ITEM_TYPE::ETC:
	case Client::ITEM_TYPE::LAST:
	default:
		break;
	}
	return listSlotID;
}


CUIPlayerInfo* CUIPlayerInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPlayerInfo* pInstance = new CUIPlayerInfo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIPlayerInfo");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIPlayerInfo::Clone(void* pArg)
{
	CUIPlayerInfo* pInstance = new CUIPlayerInfo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIPlayerInfo");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIPlayerInfo::Free()
{
	__super::Free();
}

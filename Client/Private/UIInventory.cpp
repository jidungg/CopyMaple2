#include "stdafx.h"
#include "UIInventory.h"
#include "GameInstance.h"
#include "Inventory.h"
#include "UIListSelector.h"
#include "UIScroller.h"
#include "InvenSlot.h"
#include "UIInvenSlotEntry.h"

//TODO : 슬롯  SRV 이상함
//TODO : BackPanel 이미지 이상함.
CUIInventory::CUIInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUICommonWindow(pDevice, pContext)
{
}

CUIInventory::CUIInventory(const CUIInventory& Prototype)
	: CUICommonWindow(Prototype)
{
}


HRESULT CUIInventory::Initialize(void* pArg)
{

	UIINVENTORY_DESC* pInvenDesc = static_cast<UIINVENTORY_DESC*>(pArg);
	pInvenDesc->bDraggableX = true;
	pInvenDesc->bDraggableY = true;
	pInvenDesc->eAnchorType = CORNOR_TYPE::CENTER;
	pInvenDesc->ePivotType = CORNOR_TYPE::CENTER;
	pInvenDesc->fSizeX = m_iVisibleColCount * (m_fSlotSize.x + m_fCommonMargin.x) + m_fCommonMargin.x*5 + m_fTabButtonSize.x + m_fCommonButtonSize;
	pInvenDesc->fSizeY = m_fHeaderHeight + m_iVisibleRowCount* (m_fSlotSize.y + m_fCommonMargin.y) + m_fCommonMargin.y*3;
	pInvenDesc->fXOffset = 0;
	pInvenDesc->fYOffset = 0;
	pInvenDesc->vBorder = { m_fHeaderHeight,m_fCommonMargin.y,m_fCommonMargin.x,m_fCommonMargin.x };
	pInvenDesc->szIconProtoTag = TEXT("Inventory_Icon.dds");
	pInvenDesc->szTitleText = TEXT("소지품");
	pInvenDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("Common_Window.dds")));
	if (FAILED(CUICommonWindow::Initialize(pArg)))
		return E_FAIL;


	m_pInventory = pInvenDesc->pInventory;



	return S_OK;
}
HRESULT CUIInventory::Ready_Slots()
{
	CUIPanel::PANEL_DESC tItemBackPanelDesc;
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	tItemBackPanelDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tItemBackPanelDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	tItemBackPanelDesc.fXOffset = -m_fCommonMargin.x;
	tItemBackPanelDesc.fYOffset = m_fHeaderHeight + m_fCommonMargin.y;
	tItemBackPanelDesc.fSizeX = fSize.x - m_fCommonMargin.x * 3 - m_fTabButtonSize.x;
	tItemBackPanelDesc.fSizeY = fSize.y - m_fHeaderHeight - m_fCommonMargin.y*2;
	tItemBackPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_ItemListBack"), nullptr));
	tItemBackPanelDesc.vBorder = { m_fCommonMargin.y,m_fCommonMargin.y,m_fCommonMargin.x,m_fCommonMargin.x };
	m_pItemBackPanel = static_cast<CUIPanel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIPanel::m_szProtoTag, &tItemBackPanelDesc));
	Add_Child(m_pItemBackPanel);


	CUIListSelector::UILISTSELECTOR_DESC ListDesc{};
	ListDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	ListDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	ListDesc.fXOffset = m_fCommonMargin.x;
	ListDesc.fYOffset = m_fCommonMargin.y;
	ListDesc.fSizeX = tItemBackPanelDesc.fSizeX - m_fCommonButtonSize - m_fCommonMargin.x * 3;
	ListDesc.fSizeY = tItemBackPanelDesc.fSizeY - m_fCommonMargin.y * 2;
	ListDesc.fItemHeight = m_fSlotSize.y;
	ListDesc.fItemWidth = m_fSlotSize.x;
	ListDesc.fItemMarginX = m_fCommonMargin.x;
	ListDesc.fItemMarginY = m_fCommonMargin.y;
	ListDesc.iColumnCount = m_iVisibleColCount;

	ListDesc.iRowCount = (_uint)ceilf((_float)m_pInventory->Get_Slots(m_eCurrentTab) ->size()/ (_float)m_iVisibleColCount);;
	ListDesc.eHighlighterTexProtoLev = LEVEL_LOADING;
	ListDesc.szHighlighterTexProtoTag = TEXT("UI_Texture_HighlightBorder");
	vector< CInvenSlot*>* vecSlots = m_pInventory->Get_Slots(m_eCurrentTab);
	list<ITEM_DATA*> listData;
	for (auto& pSlot : *vecSlots)
	{
		listData.push_back(pSlot->Get_ItemData());
	}
	ListDesc.listData = &listData;
	ListDesc.eItemEntryProtoLev = LEVEL_LOADING;
	ListDesc.szItemEntryProtoTag =CUIInvenSlotEntry::m_szProtoTag;
	m_pItemList = static_cast<CUIListSelector*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, TEXT("Prototype_GameObject_UIList"), &ListDesc));
	m_pItemBackPanel->Add_Child(m_pItemList);

	
	CUIScroller::SCROLLBAR_DESC ScrollDesc{};
	ScrollDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	ScrollDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	ScrollDesc.fXOffset = -m_fCommonMargin.x;
	ScrollDesc.fYOffset = -m_fCommonMargin.y;
	ScrollDesc.fSizeX = m_fCommonButtonSize;
	ScrollDesc.fSizeY = ListDesc.fSizeY;
	ScrollDesc.iTotalRowCount = m_pItemList->Get_RowCount();
	ScrollDesc.iVisibleRowCount = m_iVisibleRowCount;
	ScrollDesc.pUIList = m_pItemList;
	m_pScroller = static_cast<CUIScroller*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIScroller::m_szProtoTag, &ScrollDesc));
	m_pItemBackPanel->Add_Child(m_pScroller);

	Set_InventoryTab(ITEM_TYPE::EQUIP);


	//_float3 fSize = m_pTransformCom->Compute_Scaled();
	//_uint iXMaxCount = _uint(fSize.x / (m_fSlotSize.x + m_fSlotMargin.x));
	//_uint iYMaxCount = _uint(fSize.y / (m_fSlotSize.y + m_fSlotMargin.y));
	//
	//auto pSlots = m_pInventory->Get_Slots(m_eCurrentTab);
	//CUIInvenSlot::UIINVENSLOT_DESC tSlotDesc;
	//for (_uint iTabIdx = 0; iTabIdx < 1; iTabIdx++)
	//{
	//	_uint iSlotCount = pSlots->size();
	//	m_vecSlot[iTabIdx].reserve(iSlotCount);

	//	tSlotDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	//	tSlotDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	//	tSlotDesc.fSizeX = m_fSlotSize.x;
	//	tSlotDesc.fSizeY = m_fSlotSize.y;
	//	tSlotDesc.fXOffset = 0;
	//	tSlotDesc.fYOffset = 0;
	//	tSlotDesc.vBorder = { 3,3,3,3 };
	//	_float fSlotsOffsetX = m_vBorder.z + m_fSlotMargin.x / 2;
	//	_float fSlotsOffsetY = m_vBorder.x + m_fSlotMargin.y / 2;
	//	for (_uint i = 0; i < iSlotCount; i++)
	//	{
	//		tSlotDesc.fXOffset = fSlotsOffsetX+ i % iXMaxCount * (m_fSlotSize.x + m_fSlotMargin.x) + m_fSlotMargin.x / 2;
	//		tSlotDesc.fYOffset = fSlotsOffsetY + i / iXMaxCount * (m_fSlotSize.y + m_fSlotMargin.y) + m_fSlotMargin.y / 2;
	//		tSlotDesc.pInvenSlot = pSlots->at(i);
	//		tSlotDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("Texture_InvenSlotBack")));

	//		CUIInvenSlot* pSlot =  static_cast<CUIInvenSlot*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIInvenSlot::m_szProtoTag, &tSlotDesc));
	//		Add_Child(pSlot);
	//		pSlot->Compute_Matrix();
	//		m_vecSlot[iTabIdx].push_back(pSlot);
	//		m_vecSlot[iTabIdx][i]->Update_Slot();
	//	}
	//}

	return S_OK;
}

void CUIInventory::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIInventory::Set_InventoryTab(ITEM_TYPE eType)
{
	//탭이 바뀌면 아이템 리스트 정보도 바뀜..
	//바뀐 아이템 정보에 따라 슬롯 최대 갯수도 바뀜.
	//슬롯 최대 갯수가 바뀜에 따라 스크롤 바 크기도 바뀜.
	m_eCurrentTab = eType;
	vector< CInvenSlot*>* vecSlots = m_pInventory->Get_Slots(m_eCurrentTab);
	_uint iSlotCount = vecSlots->size();

	//콜백함수 다 지우기
	Clear_OnRightClickCallback();

	//UI 리스트 사이즈 조정
	m_pItemList->Resize((iSlotCount+1) / m_iVisibleColCount, m_iVisibleColCount);

	//데이터 셋팅
	list<ITEM_DATA*> listData;
	for (auto& pSlot : *vecSlots)
		listData.push_back(pSlot->Get_ItemData());
	m_pItemList->Set_ItemData(&listData);

	//콜백 다시 셋팅
	for (_uint i = 0; i < iSlotCount; i++)
	{
		function<void(void*)> f = bind(&CInvenSlot::On_RightClick, (*vecSlots)[i], placeholders::_1);
		m_pItemList->Register_OnRightClickCallback(i, f);
	}

}

void CUIInventory::Update_Slot(_uint iIndex, ITEM_DATA* pData)
{
	m_pItemList->Set_ItemData(iIndex, pData);
}

void CUIInventory::Clear_OnRightClickCallback()
{
	m_pItemList->Clear_OnRightClickCallback();
}



CUIInventory* CUIInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIInventory* pInstance = new CUIInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIInventory");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
CGameObject* CUIInventory::Clone(void* pArg)
{
	CUIInventory* pInstance = new CUIInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIInventory");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
void CUIInventory::Free()
{
	__super::Free();
}

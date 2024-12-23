#include "stdafx.h"
#include "UIInventory.h"
#include "GameInstance.h"
#include "Inventory.h"
#include "UIInvenItemList.h"
#include "UIScroller.h"
#include "InvenSlot.h"
#include "UIInvenSlotEntry.h"
#include "UIInvenTabButton.h"

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
	pInvenDesc->fSizeX = m_iVisibleColCount * (m_fSlotSize.x + m_fCommonMargin.x) + m_fCommonMargin.x*4 + m_fTabButtonSize.x + m_fCommonButtonSize;
	pInvenDesc->fSizeY = m_fHeaderHeight + m_iVisibleRowCount* (m_fSlotSize.y + m_fCommonMargin.y) + m_fCommonMargin.y*3;
	pInvenDesc->fXOffset = 0;
	pInvenDesc->fYOffset = 0;
	pInvenDesc->vBorder = { m_fHeaderHeight,m_fCommonMargin.y,m_fCommonMargin.x,m_fCommonMargin.x };
	pInvenDesc->szIconProtoTag = TEXT("Inventory_Icon.dds");
	pInvenDesc->szTitleText = TEXT("소지품");
	if (FAILED(CUICommonWindow::Initialize(pArg)))
		return E_FAIL;


	m_pInventory = pInvenDesc->pInventory;



	return S_OK;
}
void CUIInventory::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}
HRESULT CUIInventory::Ready_Slots()
{
	CUIPanel::PANEL_DESC tItemBackPanelDesc;
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	tItemBackPanelDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	tItemBackPanelDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	tItemBackPanelDesc.fXOffset = -m_fCommonMargin.x;
	tItemBackPanelDesc.fYOffset = m_fHeaderHeight + m_fCommonMargin.y;
	tItemBackPanelDesc.fSizeX = fSize.x - m_fCommonMargin.x * 2 - m_fTabButtonSize.x;
	tItemBackPanelDesc.fSizeY = fSize.y - m_fHeaderHeight - m_fCommonMargin.y*2;
	tItemBackPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_ItemListBack"), nullptr));
	tItemBackPanelDesc.vBorder = { m_fCommonMargin.y,m_fCommonMargin.y,m_fCommonMargin.x,m_fCommonMargin.x };
	m_pItemBackPanel = static_cast<CUIPanel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIPanel::m_szProtoTag, &tItemBackPanelDesc));
	Add_Child(m_pItemBackPanel);


	CUIListSelector::UILISTSELECTOR_DESC tListDesc{};
	tListDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tListDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tListDesc.fXOffset = m_fCommonMargin.x;
	tListDesc.fYOffset = m_fCommonMargin.y;
	tListDesc.fSizeX = tItemBackPanelDesc.fSizeX - m_fCommonButtonSize - m_fCommonMargin.x * 3;
	tListDesc.fSizeY = tItemBackPanelDesc.fSizeY - m_fCommonMargin.y * 2;
	tListDesc.fItemHeight = m_fSlotSize.y;
	tListDesc.fItemWidth = m_fSlotSize.x;
	tListDesc.fItemMarginX = m_fCommonMargin.x;
	tListDesc.fItemMarginY = m_fCommonMargin.y;
	tListDesc.iColumnCount = m_iVisibleColCount;
	tListDesc.iRowCount = (_uint)ceilf((_float)m_pInventory->Get_Slots(m_eCurrentTab) ->size()/ (_float)m_iVisibleColCount);;
	tListDesc.eHighlighterTexProtoLev = LEVEL_LOADING;
	tListDesc.szHighlighterTexProtoTag = TEXT("UI_Texture_HighlightBorder");
	tListDesc.eItemEntryProtoLev = LEVEL_LOADING;
	tListDesc.szItemEntryProtoTag =CUIInvenSlotEntry::m_szProtoTag;
	m_pItemList = static_cast<CUIInvenItemList*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, TEXT("Prototype_GameObject_CUIInvenItemList"), &tListDesc));
	m_pItemBackPanel->Add_Child(m_pItemList);

	
	CUIScroller::SCROLLBAR_DESC tScrollDesc{};
	tScrollDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	tScrollDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	tScrollDesc.fXOffset = -m_fCommonMargin.x;
	tScrollDesc.fYOffset = -m_fCommonMargin.y;
	tScrollDesc.fSizeX = m_fCommonButtonSize;
	tScrollDesc.fSizeY = tListDesc.fSizeY;
	tScrollDesc.iTotalRowCount = m_pItemList->Get_RowCount();
	tScrollDesc.iVisibleRowCount = m_iVisibleRowCount;
	tScrollDesc.pUIList = m_pItemList;
	m_pScroller = static_cast<CUIScroller*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIScroller::m_szProtoTag, &tScrollDesc));
	m_pItemBackPanel->Add_Child(m_pScroller);


	CUIInvenTabButton::UIINVENTABBUTTON_DESC tTabButtonDesc;
	tTabButtonDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tTabButtonDesc.fXOffset = m_fCommonMargin.x;
	tTabButtonDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tTabButtonDesc.fSizeX = m_fTabButtonSize.x;
	tTabButtonDesc.fSizeY = m_fTabButtonSize.y;
	tTabButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_TabButton"), nullptr));
	for (_uint i = 0; i < (_uint)ITEM_TYPE::LAST; i++)
	{
		tTabButtonDesc.fYOffset = m_fCommonMargin.y +m_fHeaderHeight + m_fTabButtonSize.y*i;
		tTabButtonDesc.pUIInventory = this;
		tTabButtonDesc.eItemType = (ITEM_TYPE)i;
		m_arrTabButton[i] = static_cast<CUIInvenTabButton*>( m_pGameInstance->Clone_Proto_Object_Stock(CUIInvenTabButton::m_szProtoTag,&tTabButtonDesc));
		Add_Child(m_arrTabButton[i]);
	}

	Set_InventoryTab(ITEM_TYPE::EQUIP);
	return S_OK;
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

	//UI 리스트 & 스크롤러 사이즈 조정
	_uint iNewRowCount = (iSlotCount + 1) / m_iVisibleColCount;
	m_pItemList->Resize(iNewRowCount, m_iVisibleColCount);
	m_pItemList->Select_Item(0);
	m_pScroller->Set_RowCounts(iNewRowCount,m_iVisibleRowCount);
	m_pScroller->Set_CursorRow(0);

	//데이터 셋팅
	list<pair<const ITEM_DATA*, _uint>> listData;
	for (auto& pSlot : *vecSlots)
	{
		listData.push_back({ pSlot->Get_ItemData() ,pSlot->Get_StackCount()});
	}
 	m_pItemList->Set_ItemData(&listData);

	//콜백 다시 셋팅
	for (_uint i = 0; i < iSlotCount; i++)
	{
		function<void(void*)> f = bind(&CInvenSlot::On_RightClick, (*vecSlots)[i], placeholders::_1);
		m_pItemList->Register_OnRightClickCallback(i, f);
	}

	for (auto& pTabButton : m_arrTabButton)
	{
		pTabButton->Set_Selected(m_eCurrentTab == pTabButton->Get_ItemType());
	}
}

void CUIInventory::Update_Slot(_uint iIndex, CInvenSlot* pSlot)
{
	if (nullptr == pSlot)
		return;
	const ITEM_DATA* pData = pSlot->Get_ItemData();
	if(nullptr != pData && m_eCurrentTab != pData->eITemType)
		return;
	_uint iCount = pSlot->Get_StackCount();
	if (iCount <= 0) pData = nullptr;
	m_pItemList->Set_ItemData(iIndex, { pData ,iCount });
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

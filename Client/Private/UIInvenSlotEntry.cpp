#include "stdafx.h"
#include "UIInvenSlotEntry.h"
#include "GameInstance.h"
#include "UIIcon.h"
#include "InvenSlot.h"
#include "UIListSelector.h"
#include "UIFont.h"
#include "UIQuickSlot.h"
#include "InvenConsumableSlot.h"

CUIInvenSlotEntry::CUIInvenSlotEntry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIButton(pDevice, pContext)
{

}

CUIInvenSlotEntry::CUIInvenSlotEntry(const CUIInvenSlotEntry& Prototype)
	: CUIButton(Prototype)
{

}


HRESULT CUIInvenSlotEntry::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


    return S_OK;
}

HRESULT CUIInvenSlotEntry::Initialize(void* pArg)
{

	UIInvenSlotEntryDesc* pDesc = static_cast<UIInvenSlotEntryDesc*>(pArg);
	pDesc->pTextureCom =static_cast<CTexture*>( m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Texture_InvenSlotBack")));
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float3 fSize = m_pTransformCom->Compute_Scaled();
	CUIPanel::PANEL_DESC tPanelDesc{};
	tPanelDesc.bDraggableX = true;
	tPanelDesc.bDraggableY = true;
	tPanelDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tPanelDesc.ePivotType = CORNOR_TYPE::CENTER;
	tPanelDesc.fSizeX = fSize.x - 10;
	tPanelDesc.fSizeY = fSize.y - 10;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fYOffset = 0;
	m_pIcon = static_cast<CUIIcon*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIIcon::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pIcon);

	CUIFont::UIFontDesc tFontDesc{};
	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	tFontDesc.fXOffset = -3.5;
	tFontDesc.fYOffset = -3.5;
	tFontDesc.pFontTag = L"LV2Gothic_Medium_10";
	tFontDesc.vColor = _vector{ 1.f, 1.f, 1.f, 1.f };
	tFontDesc.bShade = true;
	m_pStackCountFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pStackCountFont);

    return S_OK;
}

void CUIInvenSlotEntry::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

HRESULT CUIInvenSlotEntry::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

    return S_OK;
}

HRESULT CUIInvenSlotEntry::On_InvenListItemDataSet(CInvenSlot* pSlot)
{
	m_pInvenSlot = pSlot;
	const ITEM_DATA* pData = m_pInvenSlot->Get_ItemData();
	_uint iStackCount = m_pInvenSlot->Get_StackCount();
	if (iStackCount  == 0)
	{
		if (FAILED(On_ListItemDataSet(nullptr)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(On_ListItemDataSet(pData)))
			return E_FAIL;
	}
	Set_StackCount(iStackCount);
	return S_OK;
}

HRESULT CUIInvenSlotEntry::On_ListItemDataSet(const ITEM_DATA* data)
{
	m_pItemDesc = data;
	if (nullptr == m_pItemDesc )
	{
		for (auto& i : m_arrSRVIndex)
			i = 0;
		m_iSRVIndex = m_arrSRVIndex[0];
		m_pIcon->Set_Texture(nullptr);
		m_pIcon->Set_Active(false);
		m_pStackCountFont->Set_Active(false);
		return S_OK;
	}

	//아이템 등급에 따른 배경 이미지
	for (auto& i : m_arrSRVIndex)
		i = (_uint)m_pItemDesc->eItemGrade;
	m_iSRVIndex = m_arrSRVIndex[0];

	//아이콘 이미지
	CTexture* pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, m_pItemDesc->szIconImageTag, nullptr));
	m_pIcon->Set_Texture(pTexture);
	m_pIcon->Set_Active(true);

	//스택 카운트 폰트
	if (m_pItemDesc->eITemType == ITEM_TYPE::CONSUMABLE 
		|| m_pItemDesc->eITemType == ITEM_TYPE::ETC
		|| m_pItemDesc->eITemType == ITEM_TYPE::BUILD)
		m_pStackCountFont->Set_Active(true);
	else
		m_pStackCountFont->Set_Active(false);

	Set_Disable(false);
	return S_OK;
}



HRESULT CUIInvenSlotEntry::Render_ListEntry()
{
	if (Is_Valid())
		return Render();
    return S_OK;
}

void CUIInvenSlotEntry::On_CreateListItemEntry(CUIList* pList, _uint iIndex)
{
	__super::On_CreateListItemEntry(pList, iIndex);
	Set_Disable(false);
	m_pList = pList;
}

void CUIInvenSlotEntry::On_MouseEnter()
{
	__super::On_MouseEnter();
	static_cast<CUIListSelector*>( m_pList)->Select_Item(Get_ListItemIndex());
}

void CUIInvenSlotEntry::On_MouseRightClick()
{
	__super::On_MouseRightClick();
}

void CUIInvenSlotEntry::On_Dropped(CUIObject* pFloorObject)
{
	CUIQuickSlot* pFloor = dynamic_cast<CUIQuickSlot*>(pFloorObject);
	if (pFloor)
	{
		if (m_pItemDesc)
		{
			if (m_pItemDesc->eITemType == ITEM_TYPE::CONSUMABLE)
			{
				pFloor->Set_QuickItem(static_cast<CInvenConsumableSlot*>(m_pInvenSlot));
			}
		}
	}

	CRect_Transform* pTransform = static_cast<CRect_Transform*>(m_pIcon->Get_Transform());
	pTransform->Set_Offset(0, 0);
	Compute_Matrix_Recursive();
	return;
}

void CUIInvenSlotEntry::On_MouseDrag(const POINT& tMousePoint, const DIMOUSESTATE& tState)
{
	if (m_bGrabbed == false)
		return;
	CRect_Transform* pTransform = static_cast<CRect_Transform*>(m_pIcon->Get_Transform());
	_float2 vMovedPosition = pTransform->Get_Offset();
	vMovedPosition.x = (_float)m_tGrabbedOffset.x + (_float)tMousePoint.x - (_float)m_tGrabbedMousePos.x;
	vMovedPosition.y = (_float)m_tGrabbedOffset.y + (_float)tMousePoint.y - (_float)m_tGrabbedMousePos.y;

	pTransform->Set_Offset(vMovedPosition.x, vMovedPosition.y);

	Compute_Matrix_Recursive();
	return;
}



void CUIInvenSlotEntry::Set_ListItemEntryActive(_bool bActive)
{
	Set_Active(bActive);
}
void CUIInvenSlotEntry::Set_Offset(_float iX, _float iY)
{
	static_cast<CRect_Transform*>(m_pTransformCom)->Set_Offset(iX, iY);
}

void CUIInvenSlotEntry::Set_StackCount(_uint iCount)
{
	wstring str = to_wstring(iCount);
	m_pStackCountFont->Set_Text(str.data());
}	



CUIInvenSlotEntry* CUIInvenSlotEntry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIInvenSlotEntry* pInstance = new CUIInvenSlotEntry(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIInvenSlotEntry");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIInvenSlotEntry::Clone(void* pArg)
{
	CUIInvenSlotEntry* pInstance = new CUIInvenSlotEntry(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIInvenSlotEntry");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIInvenSlotEntry::Free()
{
	__super::Free();

}



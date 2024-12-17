#include "stdafx.h"
#include "UIInvenSlotEntry.h"
#include "GameInstance.h"
#include "UIIcon.h"
#include "InvenSlot.h"
#include "UIListSelector.h"

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

	tPanelDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tPanelDesc.ePivotType = CORNOR_TYPE::CENTER;
	tPanelDesc.fSizeX = fSize.x - 10;
	tPanelDesc.fSizeY = fSize.y - 10;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fYOffset = 0;
	m_pIcon = static_cast<CUIIcon*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIIcon::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pIcon);

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

HRESULT CUIInvenSlotEntry::On_ListItemDataSet(const ITEM_DATA* data)
{
	if (nullptr == data)
	{
		for (auto& i : m_arrSRVIndex)
			i = 0;
		m_iSRVIndex = m_arrSRVIndex[0];
		m_pIcon->Set_Active(false);
		return S_OK;
	}
	m_pItemDesc = data;
	for (auto& i : m_arrSRVIndex)
		i = (_uint)m_pItemDesc->eItemGrade;
	m_iSRVIndex = m_arrSRVIndex[0];
	string strProtoItemIconTag = m_pItemDesc->strIconImageTag;
	wstring wstrItemIconTag(strProtoItemIconTag.begin(), strProtoItemIconTag.end());
	CTexture* pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, wstrItemIconTag, nullptr));
	m_pIcon->Set_Texture(pTexture);
	m_pIcon->Set_Active(true);
	Set_Disable(false);
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


void CUIInvenSlotEntry::Set_ListItemEntryActive(_bool bActive)
{
	Set_Active(bActive);
}
void CUIInvenSlotEntry::Set_Offset(_float iX, _float iY)
{
	static_cast<CRect_Transform*>(m_pTransformCom)->Set_Offset(iX, iY);
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



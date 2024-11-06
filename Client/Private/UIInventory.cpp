#include "stdafx.h"
#include "UIInventory.h"
#include "GameInstance.h"
#include "Inventory.h"

CUIInventory::CUIInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CUIInventory::CUIInventory(const CUIInventory& Prototype)
	: CUIPanel(Prototype)
{
}

HRESULT CUIInventory::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUIInventory::Initialize(void* pArg)
{
	UIINVENTORY_DESC* pInvenDesc = static_cast<UIINVENTORY_DESC*>(pArg);
	pInvenDesc->eAnchorType = CORNOR_TYPE::CENTER;
	pInvenDesc->ePivotType = CORNOR_TYPE::CENTER;
	pInvenDesc->fSizeX = 400;
	pInvenDesc->fSizeY = 600;
	pInvenDesc->fXOffset = 0;
	pInvenDesc->fYOffset = 0;

	pInvenDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("Common_Window.dds")));
	if (FAILED(CUIPanel::Initialize(pArg)))
		return E_FAIL;


	m_pInventory = pInvenDesc->pInventory;
	m_fSlotSize = { 60,60 };
	m_fSlotMargin = { 5,5 };

	Set_InventoryTab(ITEM_TYPE::EQUIP);
	return S_OK;
}
HRESULT CUIInventory::Ready_Slots()
{
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	_uint iXMaxCount = _uint(fSize.x / m_fSlotSize.x);
	_uint iYMaxCount = _uint(fSize.y / m_fSlotSize.y);
	
	auto pSlots = m_pInventory->Get_Slots(m_eCurrentTab);
	CUIInvenSlot::UIINVENSLOT_DESC tSlotDesc;
	for (_uint iTabIdx = 0; iTabIdx < 1; iTabIdx++)
	{
		_uint iSlotCount = pSlots->size();
		m_vecSlot[iTabIdx].reserve(iSlotCount);

		tSlotDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
		tSlotDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
		tSlotDesc.fSizeX = m_fSlotSize.x;
		tSlotDesc.fSizeY = m_fSlotSize.y;

		for (_uint i = 0; i < iSlotCount; i++)
		{
			tSlotDesc.fXOffset = i % iXMaxCount * (m_fSlotSize.x + m_fSlotMargin.x) + m_fSlotMargin.x / 2;
			tSlotDesc.fYOffset = i / iXMaxCount * (m_fSlotSize.y + m_fSlotMargin.y) + m_fSlotMargin.y / 2;
			tSlotDesc.fSizeX = m_fSlotSize.x;
			tSlotDesc.fSizeY = m_fSlotSize.y;
			tSlotDesc.pInvenSlot = pSlots->at(i);
			tSlotDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("Texture_InvenSlotBack")));
			tSlotDesc.pParent = this;

			CUIInvenSlot* pSlot =  static_cast<CUIInvenSlot*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIInvenSlot::m_szProtoTag, &tSlotDesc));
			Add_Child(pSlot);
			pSlot->Compute_Matrix();
			m_vecSlot[iTabIdx].push_back(pSlot);
			m_vecSlot[iTabIdx][i]->Update_Slot();
		}
	}

	return S_OK;
}

void CUIInventory::Update(_float fTimeDelta)
{
	int a = 0;
}

void CUIInventory::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIInventory::Set_InventoryTab(ITEM_TYPE eType)
{
	for (_uint iTabIdx = 0; iTabIdx < (_uint)ITEM_TYPE::LAST; iTabIdx++)
		for (auto& slot : m_vecSlot[iTabIdx])
			slot->Set_Active(iTabIdx == (_uint)eType);
}

void CUIInventory::Update_Slot(ITEM_TYPE eType, _uint iIndex)
{
	m_vecSlot[(_uint)eType][iIndex]->Update_Slot();
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

#include "stdafx.h"
#include "UIInvenSlot.h"
#include "GameInstance.h"
#include "InvenSlot.h"

CUIInvenSlot::CUIInvenSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUISlot(pDevice, pContext)
{
}

CUIInvenSlot::CUIInvenSlot(const CUIInvenSlot& Prototype)
	: CUISlot(Prototype)
	, m_pSlot(Prototype.m_pSlot)
{
	Safe_AddRef(m_pSlot);
}

HRESULT CUIInvenSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIInvenSlot::Initialize(void* pArg)
{
	UIINVENSLOT_DESC* pDesc = static_cast<UIINVENSLOT_DESC*>(pArg);
	m_pSlot = pDesc->pInvenSlot;
	Safe_AddRef(m_pSlot);
	pDesc->tIconDesc.eAnchorType = CORNOR_TYPE::CENTER;
	pDesc->tIconDesc.ePivotType = CORNOR_TYPE::CENTER;
	pDesc->tIconDesc.fSizeX = pDesc->fSizeX ;
	pDesc->tIconDesc.fSizeY = pDesc->fSizeY;
	pDesc->tIconDesc.fXOffset = 0;
	pDesc->tIconDesc.fYOffset = 0;

	if(FAILED( __super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CUIInvenSlot::Update_Slot()
{
	assert(m_pSlot != nullptr);

	if (m_pSlot->Is_Empty())
	{
		m_iSRVIndex = 0;
		Set_IconTexture(nullptr);
		return;
	}
	auto pITemDesc = m_pSlot->Get_ItemData();
	m_iSRVIndex = (_uint)pITemDesc->eItemGrade;
	string str = pITemDesc->strIconImageTag;
	wstring wstr(str.begin(), str.end());
	CTexture* pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, wstr));
	Set_IconTexture(pTextureCom);
}
CUIInvenSlot* CUIInvenSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIInvenSlot* pInstance = new CUIInvenSlot(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CUIInventory");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUIInvenSlot::Clone(void* pArg)
{
	CUIInvenSlot* pInstance = new CUIInvenSlot(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIInventory");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUIInvenSlot::Free()
{
	__super::Free();
	Safe_Release(m_pSlot);

}

bool CUIInvenSlot::Has_Item()
{
	return m_pSlot != nullptr && false == m_pSlot->Is_Empty();
}



void CUIInvenSlot::On_MouseClick()
{
	//if (m_pSlot)
	//{
	//	m_pSlot->On_LeftClick();
	//}

}

void CUIInvenSlot::On_MouseRightClick()
{
	//if (m_pSlot)
	//{
	//	m_pSlot->On_RightClick();
	//}

}



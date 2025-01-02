#include "stdafx.h"
#include "UIInvenTabButton.h"
#include "UIFont.h"
#include "GameInstance.h"
#include "UIInventory.h"

 const _tchar* CUIInvenTabButton::m_szTabName[(_uint)ITEM_TYPE::LAST] = {
		L"장비",
		L"치장",
		L"소비",
		L"건설" ,
		L"기타",
};

CUIInvenTabButton::CUIInvenTabButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIButton(pDevice, pContext)
{
}

CUIInvenTabButton::CUIInvenTabButton(const CUIInvenTabButton& Prototype)
	:CUIButton(Prototype)
{
}

HRESULT CUIInvenTabButton::Initialize(void* pArg)
{
	UIInvenTabButtonDesc* pDesc = static_cast<UIInvenTabButtonDesc*>(pArg);
	m_eItemType = pDesc->eItemType;
	m_pUIInventory = pDesc->pUIInventory;

	m_arrSRVIndex[BS_NORMAL] = -1;
	m_arrSRVIndex[BS_HIGHLIGHTED] = 0;
	m_arrSRVIndex[BS_PRESSED] = 2;
	m_arrSRVIndex[BS_DISABLED] = -1;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	_float3 fSize = m_pTransformCom->Compute_Scaled();

	CUIFont::UIFontDesc tFontDesc{};
	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.fXOffset = 2.5;
	tFontDesc.fYOffset =  2.5;
	tFontDesc.pText = m_szTabName[(_uint)m_eItemType];
	tFontDesc.pFontTag = L"LV2Gothic_Medium_15";
	tFontDesc.vColor = _vector{ 0.f, 0.f, 0.f, 1.f };
	m_pFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pFont);

	return S_OK;
}

HRESULT CUIInvenTabButton::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

void CUIInvenTabButton::On_MouseClick()
{
	__super::On_MouseClick();
	m_pUIInventory->Set_InventoryTab(m_eItemType);
}


void CUIInvenTabButton::On_MouseEnter()
{
	if (m_bDisabled)return;
	if (m_iSRVIndex != 1)
		m_iSRVIndex = m_arrSRVIndex[BS_HIGHLIGHTED];

}

void CUIInvenTabButton::On_MouseExit()
{
	if (m_bDisabled)return;
	if (m_iSRVIndex != 1)
		m_iSRVIndex = m_arrSRVIndex[BS_NORMAL];
}

void CUIInvenTabButton::On_MouseLButtonDown(const POINT& tMousePoint)
{
	if (m_bDisabled)return;
	if (m_iSRVIndex != 1)
		m_iSRVIndex = m_arrSRVIndex[BS_PRESSED];
}

void CUIInvenTabButton::On_MouseLButtonUp(const POINT& tMousePoint)
{
	if (m_bDisabled)return;
	if (m_iSRVIndex != 1)
		m_iSRVIndex = m_arrSRVIndex[BS_NORMAL];
}


void CUIInvenTabButton::Set_Selected(_bool bSelected)
{
	if (bSelected)
	{
		m_iSRVIndex = 1;
	}
	else
	{
		m_iSRVIndex = -1;
	}
}

CUIInvenTabButton* CUIInvenTabButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIInvenTabButton* pInstance = new CUIInvenTabButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIInvenTabButton");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIInvenTabButton::Clone(void* pArg)
{
	CUIInvenTabButton* pInstance = new CUIInvenTabButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIInvenTabButton");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIInvenTabButton::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "UIScroller.h"
#include "UIScrollButton.h"
#include "GameInstance.h"
#include "UIList.h"

CUIScroller::CUIScroller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainer(pDevice, pContext)
{
}

CUIScroller::CUIScroller(const CUIScroller& Prototype)
	: CUIContainer(Prototype)
{
}


HRESULT CUIScroller::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	SCROLLBAR_DESC* pScrollBarDesc = static_cast<SCROLLBAR_DESC*>(pArg);
	m_pUIList = pScrollBarDesc->pUIList;
	m_iTotalRowCount = pScrollBarDesc->iTotalRowCount;
	m_iVisibleRowCount = pScrollBarDesc->iVisibleRowCount;
	m_iCursorPosition = pScrollBarDesc->iInitialCursorPosition;


	_float3 fSize = m_pTransformCom->Compute_Scaled();
	CUIScrollButton::SCROLLBUTTON_DESC tScrollButtonDesc;
	tScrollButtonDesc.bDraggableX = false;
	tScrollButtonDesc.bDraggableY = true;
	tScrollButtonDesc.eAnchorType = CORNOR_TYPE::TOP;
	tScrollButtonDesc.ePivotType = CORNOR_TYPE::TOP;
	tScrollButtonDesc.fXOffset = 0;
	tScrollButtonDesc.fYOffset = fSize.x;
	tScrollButtonDesc.fSizeX = fSize.x;
	tScrollButtonDesc.fMaxButtonHeight = fSize.y - fSize.x*2;
	tScrollButtonDesc.fMinButtonHeight = fSize.x;
	tScrollButtonDesc.fMinOffsetY = tScrollButtonDesc.fYOffset;
	tScrollButtonDesc.fMaxOffsetY = fSize.y - fSize.x;
	tScrollButtonDesc.iTotalRowCount = m_iTotalRowCount;
	tScrollButtonDesc.iVisibleRowCount = m_iVisibleRowCount;
	tScrollButtonDesc.vBorder = { 3,3,3,3 };
	tScrollButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("UI_Texture_ScrollButton")));
	tScrollButtonDesc.pScroller = this;
	m_pScrollButton = static_cast<CUIScrollButton*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIScrollButton::m_szProtoTag, &tScrollButtonDesc));
	if (nullptr == m_pScrollButton)
		return E_FAIL;
	Add_Child(m_pScrollButton);

	CUIButton::BUTTON_DESC tButtonDesc;
	tButtonDesc.fSizeX = 24;
	tButtonDesc.fSizeY = 24;
	tButtonDesc.fXOffset= 0;
	tButtonDesc.fYOffset = 0;
	tButtonDesc.eAnchorType = CORNOR_TYPE::TOP;
	tButtonDesc.ePivotType = CORNOR_TYPE::TOP;
	tButtonDesc.pTextureCom = static_cast<CTexture*>( m_pGameInstance->Clone_Proto_Component_Stock(TEXT("UI_Texture_ScrollUpButton")));
	m_pUpButton = static_cast<CUIButton*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIButton::m_szProtoTag, &tButtonDesc));
	if (nullptr == m_pUpButton)
		return E_FAIL;
	Add_Child(m_pUpButton);
	m_pUpButton->Register_OnClickCallback(bind(&CUIScroller::Scroll_Up, this));


	tButtonDesc.eAnchorType = CORNOR_TYPE::BOT;
	tButtonDesc.ePivotType = CORNOR_TYPE::BOT;
	tButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("UI_Texture_ScrollDownButton")));
	m_pDownButton = static_cast<CUIButton*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIButton::m_szProtoTag, &tButtonDesc));
	if (nullptr == m_pDownButton)
		return E_FAIL;
	Add_Child(m_pDownButton);
	m_pDownButton->Register_OnClickCallback(bind(&CUIScroller::Scroll_Down, this));

	Set_CursorRow(m_iCursorPosition);
	return S_OK;
}

HRESULT CUIScroller::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CUIScroller::Scroll_Up()
{
	_uint iNewCursorPos = m_iCursorPosition == 0 ? 0 :m_iCursorPosition - 1;
	Set_CursorRow(iNewCursorPos);
}

void CUIScroller::Scroll_Down()
{
	_uint iNewCursorPos = min(m_iTotalRowCount - m_iVisibleRowCount, m_iCursorPosition + 1);
	Set_CursorRow(iNewCursorPos);
}


void CUIScroller::Set_RowCounts(_uint iTotalRowCount, _uint iVisibleCount)
{
	m_iTotalRowCount = iTotalRowCount;
	m_iVisibleRowCount = iVisibleCount;
	m_pScrollButton->Set_RowCounts(iTotalRowCount, iVisibleCount);
}

void CUIScroller::Set_CursorRow(_uint iRow)
{
	m_iCursorPosition = iRow;
	m_pUIList->Set_YOffset(m_pUIList ->Get_YMargin() - (m_iCursorPosition * m_pUIList->Get_HeightPerItem()));
	m_pUIList->Set_VisibleRowCount(m_iVisibleRowCount);
	m_pUIList->Set_VisibleRowStart(m_iCursorPosition);
	m_pScrollButton->Set_Offset(m_iCursorPosition);
	//Compute_Matrix_Recursive();
}



CUIScroller* CUIScroller::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIScroller* pInstance = new CUIScroller(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHomeDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIScroller::Clone(void* pArg)
{
	CUIScroller* pInstance = new CUIScroller(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIScrollBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIScroller::Free()
{
	__super::Free();
}

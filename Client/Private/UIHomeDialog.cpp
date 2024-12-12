#include "stdafx.h"
#include "UIHomeDialog.h"
#include "GameInstance.h"
#include "UIButton.h"
#include "ItemDataBase.h"
#include "UIList.h"
#include "UIScroller.h"

CUIHomeDialog::CUIHomeDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CUIHomeDialog::CUIHomeDialog(const CUIHomeDialog& Prototype)
	: CUIPanel(Prototype)
{
}

HRESULT CUIHomeDialog::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUIHomeDialog::Initialize(void* pArg)
{
	HOMEDIALOG_DESC* pDesc = static_cast<HOMEDIALOG_DESC*>(pArg);
	pDesc->fSizeX = m_fCommonMargin * (2 + m_iVisibleColCount + 1 + 1) + m_fItemWidth * m_iVisibleColCount + m_fCommonButtonSize;
	pDesc->fSizeY = m_fHeaderHeight +  m_fCommonMargin * (1 + m_iVisibleRowCount +1 ) + m_fItemHeight * m_iVisibleRowCount;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Childs( pDesc)))
		return E_FAIL;
	return S_OK;
}

void CUIHomeDialog::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUIHomeDialog::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);

	
}

_uint CUIHomeDialog::AddRef()
{
	if (m_pGameInstance->GetKeyState(KEY::B) == KEY_STATE::DOWN)
		int a = 0;
	return __super::AddRef();
}

void CUIHomeDialog::Select_Item(_uint iID)
{
	m_pItemList->Select_Item(iID);
}

void CUIHomeDialog::Select_NextItem()
{
	_uint iRow = m_pItemList->Get_ItemRow(m_pItemList->Select_NextItem());
	if (false == m_pItemList-> Is_VisibleRow(iRow))
		m_pScroller->Set_CursorRow(iRow);
}

HRESULT CUIHomeDialog::Ready_Childs(HOMEDIALOG_DESC* pDesc)
{
	CUIButton::BUTTON_DESC ButtonDesc{};
	ButtonDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	ButtonDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	ButtonDesc.fXOffset = -m_fCommonMargin;
	ButtonDesc.fYOffset = (m_fHeaderHeight - 24) / 2;
	ButtonDesc.fSizeX = m_fCommonButtonSize;
	ButtonDesc.fSizeY = m_fCommonButtonSize;
	ButtonDesc.vBorder = { 2,2,2,2 };
	ButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_HOME, TEXT("UI_Texture_Magnifier"), nullptr));

	CUIObject* pButton = CUIButton::Create(m_pDevice,m_pContext);
	if (FAILED(pButton->Initialize(&ButtonDesc)))
		return E_FAIL;
	Add_Child(pButton);

	CUIPanel::PANEL_DESC tItemBackPanelDesc;
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	tItemBackPanelDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tItemBackPanelDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tItemBackPanelDesc.fXOffset = m_fCommonMargin;
	tItemBackPanelDesc.fYOffset = m_fHeaderHeight;
	tItemBackPanelDesc.fSizeX = fSize.x - m_fCommonMargin*2;
	tItemBackPanelDesc.fSizeY = fSize.y - m_fHeaderHeight - m_fCommonMargin ;
	tItemBackPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_HOME, TEXT("UI_Texture_ItemListBack"), nullptr));
	tItemBackPanelDesc.vBorder = { 4,4,4,4 };
	m_pItemBackPanel = static_cast<CUIPanel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIPanel::m_szProtoTag, &tItemBackPanelDesc));
	Add_Child(m_pItemBackPanel);

	CUIListSelector::UILISTSELECTOR_DESC ListDesc{};
	ListDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	ListDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	ListDesc.fXOffset = m_fCommonMargin;
	ListDesc.fYOffset = m_fCommonMargin;
	ListDesc.fSizeX = tItemBackPanelDesc.fSizeX - m_fCommonButtonSize - m_fCommonMargin*3;
	ListDesc.fSizeY = tItemBackPanelDesc.fSizeY - m_fCommonMargin *2;
	ListDesc.fItemHeight = m_fItemHeight;
	ListDesc.fItemWidth = m_fItemHeight;
	ListDesc.fItemMarginX = m_fCommonMargin;
	ListDesc.fItemMarginY = m_fCommonMargin;
	ListDesc.iColumnCount = m_iVisibleColCount;
	ListDesc.iRowCount = (_uint)ceilf((_float)pDesc->listData->size() / (_float)m_iVisibleColCount);;
	ListDesc.eHighlighterTexProtoLev = LEVEL_HOME;
	ListDesc.szHighlighterTexProtoTag = TEXT("UI_Texture_HighlightBorder");
	ListDesc.listData = pDesc->listData;
	ListDesc.eBackTexProtoLev = LEVEL_HOME;
	ListDesc.szBackTexProtoTag = TEXT("Prototype_GameObject_HomeDialogBuildItemIndicator");
	m_pItemList = static_cast<CUIListSelector*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, TEXT("Prototype_GameObject_UIList"), &ListDesc));
	m_pItemBackPanel->Add_Child(m_pItemList);

	CUIScroller::SCROLLBAR_DESC ScrollDesc{};
	ScrollDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	ScrollDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	ScrollDesc.fXOffset = -m_fCommonMargin;
	ScrollDesc.fYOffset = -m_fCommonMargin;
	ScrollDesc.fSizeX = m_fCommonButtonSize;
	ScrollDesc.fSizeY = ListDesc.fSizeY;
	ScrollDesc.iTotalRowCount = m_pItemList->Get_RowCount();
	ScrollDesc.iVisibleRowCount = m_iVisibleRowCount;
	ScrollDesc.pUIList = m_pItemList;
	m_pScroller = static_cast<CUIScroller*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIScroller::m_szProtoTag, &ScrollDesc));
	m_pItemBackPanel->Add_Child(m_pScroller);

  	return S_OK;
}


CUIHomeDialog* CUIHomeDialog::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIHomeDialog* pInstance = new CUIHomeDialog(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHomeDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIHomeDialog::Clone(void* pArg)
{
	CUIHomeDialog* pInstance = new CUIHomeDialog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHomeDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIHomeDialog::Free()
{
	__super::Free();
}

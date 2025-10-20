#include "stdafx.h"
#include "UICommonWindow.h"
#include "GameInstance.h"
#include "UIButton.h"
#include "UIIcon.h"
#include "UIFont.h"

CUICommonWindow::CUICommonWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIPanel(pDevice, pContext)
{
}

CUICommonWindow::CUICommonWindow(const CUICommonWindow& Prototype)
	:CUIPanel(Prototype)
{
}


HRESULT CUICommonWindow::Initialize(void* pArg)
{
	UICOMMONWINDOW_DESC* pDesc = static_cast<UICOMMONWINDOW_DESC*> (pArg);
	pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("Common_Window.dds")));
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (nullptr != pDesc->szIconProtoTag)
	{
		CUIPanel::PANEL_DESC tPanelDesc{};
		
		tPanelDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
		tPanelDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
		tPanelDesc.fSizeX = 63;
		tPanelDesc.fSizeY = 63;
		tPanelDesc.fXOffset = m_fCommonMargin;
		tPanelDesc.fYOffset = -(tPanelDesc.fSizeY - m_vBorder.x);
		tPanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(pDesc->szIconProtoTag));
		CUIIcon* pIcon = static_cast<CUIIcon*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIIcon::m_szProtoTag,&tPanelDesc));
		Add_Child(pIcon);
		pIcon->Set_Active(false);
	}

	CUIFont::UIFontDesc FontDesc{};
	FontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	FontDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	FontDesc.fXOffset = 63 + m_fCommonMargin*2;
	FontDesc.fYOffset = (m_vBorder.x - 15) / 2;
	FontDesc.pFontTag = TEXT("LV2Gothic_Bold_15");
	FontDesc.pText = pDesc->szTitleText;
	FontDesc.bShade = true;
	CUIFont* pFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &FontDesc));
	Add_Child(pFont);
	pFont->Set_Active(false);

	CUIButton::BUTTON_DESC ButtonDesc{};
	ButtonDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	ButtonDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	ButtonDesc.fXOffset = -m_fCommonMargin*2;
	ButtonDesc.fYOffset = (m_vBorder.x - 24) / 2;
	ButtonDesc.fSizeX = m_fCommonButtonSize;
	ButtonDesc.fSizeY = m_fCommonButtonSize;
	ButtonDesc.vBorder = { 2,2,2,2 };
	ButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("UI_Texture_CloseButton"), nullptr));

	CUIButton* pButton = CUIButton::Create(m_pDevice, m_pContext);
	if (FAILED(pButton->Initialize(&ButtonDesc)))
		return E_FAIL;
	Add_Child(pButton);
	pButton->Register_OnClickCallback(bind(&CUICommonWindow::Close_Window, this));


	return S_OK;
}

void CUICommonWindow::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}


HRESULT CUICommonWindow::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	
	return S_OK;
}

void CUICommonWindow::Close_Window()
{
	Set_Active(false);
}

void CUICommonWindow::Free()
{
	__super::Free();
}
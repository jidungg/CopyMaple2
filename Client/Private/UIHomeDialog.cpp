#include "stdafx.h"
#include "UIHomeDialog.h"
#include "GameInstance.h"
#include "UIButton.h"
#include "ItemDataBase.h"
#include "UIList.h"

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


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Childs( pDesc)))
		return E_FAIL;
	return S_OK;
}

void CUIHomeDialog::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);

	
}



void CUIHomeDialog::On_MouseEnter()
{
}

void CUIHomeDialog::MouseOver()
{
	int a = 0;
}

void CUIHomeDialog::On_MouseExit()
{
}

void CUIHomeDialog::On_MouseLButtonDown()
{
	return ;
}

void CUIHomeDialog::On_MouseLButtonUp()
{
	return ;
}

void CUIHomeDialog::On_MouseRButtonDown()
{
	return ;
}

void CUIHomeDialog::On_MouseRButtonUp()
{
	return ;
}

void CUIHomeDialog::On_MouseClick()
{
	return ;
}



HRESULT CUIHomeDialog::Ready_Childs(HOMEDIALOG_DESC* pDesc)
{
	CUIButton::BUTTON_DESC ButtonDesc{};
	ButtonDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	ButtonDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	ButtonDesc.fXOffset = -50;
	ButtonDesc.fYOffset = 0;
	ButtonDesc.fSizeX = 24;
	ButtonDesc.fSizeY =24;
	ButtonDesc.vBorder = { 2,2,2,2 };
	ButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_HOME, TEXT("UI_Texture_Magnifier"), nullptr));

	CUIObject* pButton = CUIButton::Create(m_pDevice,m_pContext);
	if (FAILED(pButton->Initialize(&ButtonDesc)))
		return E_FAIL;
	Add_Child(pButton);

	CUIListSelector::UILISTSELECTOR_DESC ListDesc{};
	ListDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	ListDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	ListDesc.fXOffset = -10;
	ListDesc.fYOffset = -10;
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	ListDesc.fSizeX = fSize.x - 50;
	ListDesc.fSizeY = fSize.y - 50;
	ListDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_HOME, TEXT("UI_Texture_ItemListBack"), nullptr));;
	ListDesc.fItemHeight = 50;
	ListDesc.fItemWidth = 50;
	ListDesc.fItemMarginX = 10;
	ListDesc.fItemMarginY = 10;
	ListDesc.eBackTexProtoLev = LEVEL_HOME;
	ListDesc.szBackTexProtoTag = TEXT("Prototype_GameObject_HomeDialogBuildItemIndicator");
	ListDesc.eHighlighterTexProtoLev = LEVEL_HOME;
	ListDesc.szHighlighterTexProtoTag = TEXT("UI_Texture_HighlightBorder");
	ListDesc.listData = pDesc->listData;
	ListDesc.vBorder = { 4,4,4,4 };

	m_pItemList = static_cast<CUIListSelector*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, TEXT("Prototype_GameObject_UIList"), &ListDesc));
	Add_Child(m_pItemList);


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

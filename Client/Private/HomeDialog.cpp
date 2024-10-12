#include "stdafx.h"
#include "HomeDialog.h"
#include "GameInstance.h"
#include "UIButton.h"

CHomeDialog::CHomeDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CHomeDialog::CHomeDialog(const CHomeDialog& Prototype)
	: CUIPanel(Prototype)
{
}

HRESULT CHomeDialog::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHomeDialog::Initialize(void* pArg)
{
	PANEL_DESC* pDesc = static_cast<PANEL_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Childs()))
		return E_FAIL;
	return S_OK;
}

void CHomeDialog::On_MouseEnter()
{
}

void CHomeDialog::MouseOver()
{
	int a = 0;
}

void CHomeDialog::On_MouseExit()
{
}

void CHomeDialog::On_MouseLButtonDown()
{
	return ;
}

void CHomeDialog::On_MouseLButtonUp()
{
	return ;
}

void CHomeDialog::On_MouseRButtonDown()
{
	return ;
}

void CHomeDialog::On_MouseRButtonUp()
{
	return ;
}

void CHomeDialog::On_MouseClick()
{
	return ;
}

HRESULT CHomeDialog::Ready_Childs()
{
	CUIButton::BUTTON_DESC ButtonDesc{};
	ButtonDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	ButtonDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	ButtonDesc.fXOffset = -50;
	ButtonDesc.fYOffset = 0;
	ButtonDesc.fSizeX = 50;
	ButtonDesc.fSizeY =50;
	ButtonDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_HOME, TEXT("UI_Texture_Magnifier"), nullptr));

	CUIObject* pButton = CUIButton::Create(m_pDevice,m_pContext);
	if (FAILED(pButton->Initialize(&ButtonDesc)))
		return E_FAIL;
	Add_Child(pButton);

	return S_OK;
}


CHomeDialog* CHomeDialog::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHomeDialog* pInstance = new CHomeDialog(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHomeDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHomeDialog::Clone(void* pArg)
{
	CHomeDialog* pInstance = new CHomeDialog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHomeDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHomeDialog::Free()
{
	__super::Free();
}

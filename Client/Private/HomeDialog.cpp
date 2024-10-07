#include "stdafx.h"
#include "HomeDialog.h"
#include "GameInstance.h"

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

void CHomeDialog::On_MouseOver()
{
	int a = 0;
}

void CHomeDialog::On_MouseExit()
{
}

bool CHomeDialog::Consume_MouseLButtonDown()
{
	return false;
}

bool CHomeDialog::Consume_MouseLButtonUp()
{
	return false;
}

bool CHomeDialog::Consume_MouseRButtonDown()
{
	return false;
}

bool CHomeDialog::Consume_MouseRButtonUp()
{
	return false;
}

bool CHomeDialog::Consume_MouseClick()
{
	return false;
}

HRESULT CHomeDialog::Ready_Childs()
{
	CUIPanel::PANEL_DESC PanelDesc{};
	PanelDesc.eAnchorType = CORNOR_TYPE::RIGHT_TOP;
	PanelDesc.ePivotType = CORNOR_TYPE::RIGHT_TOP;
	PanelDesc.fXOffset = -50;
	PanelDesc.fYOffset = 0;
	PanelDesc.fSizeX = 50;
	PanelDesc.fSizeY =50;
	PanelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_HOME, TEXT("UI_Texture_Magnifier_Normal"), nullptr));

	CUIObject* pPanel = CUIPanel::Create(m_pDevice,m_pContext);
	if (FAILED(pPanel->Initialize(&PanelDesc)))
		return E_FAIL;
	Add_Child(pPanel);

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

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
	PANEL_DESC* pDesc = static_cast<PANEL_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Childs()))
		return E_FAIL;
	return S_OK;
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

HRESULT CUIHomeDialog::Ready_Childs()
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

	CUIList::UILIST_DESC ListDesc{};
	ListDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	ListDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	ListDesc.fXOffset = -10;
	ListDesc.fYOffset = -10;
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	ListDesc.fSizeX = fSize.x - 50;
	ListDesc.fSizeY = fSize.y - 50;
	ListDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_HOME, TEXT("UI_Texture_ItemListBack"), nullptr));;

	m_pItemList = static_cast<CUIList*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_HOME,
		TEXT("Prototype_GameObject_HomeDialogBuildItemList"), &ListDesc));
	auto m = ITEMDB->GetItemMap(ITEM_TYPE::BUILD);
	list<UIListItemData*> listItemDesc;
	for (auto& i : *m)
		listItemDesc.push_back(i.second);
	
	m_pItemList->Set_Data(listItemDesc);
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

#include "stdafx.h"
#include "UIInvenTabButton.h"

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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUIInvenTabButton::On_MouseClick()
{
	__super::On_MouseClick();

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

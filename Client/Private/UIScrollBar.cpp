#include "stdafx.h"
#include "UIScrollBar.h"

CUIScrollBar::CUIScrollBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CUIScrollBar::CUIScrollBar(const CUIPanel& Prototype)
	: CUIPanel(Prototype)
{
}

CUIScrollBar* CUIScrollBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIScrollBar* pInstance = new CUIScrollBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHomeDialog");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIScrollBar::Clone(void* pArg)
{
	CUIScrollBar* pInstance = new CUIScrollBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIScrollBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIScrollBar::Free()
{
	__super::Free();
}

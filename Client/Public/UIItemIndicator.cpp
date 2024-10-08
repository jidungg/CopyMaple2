#include "stdafx.h"
#include "UIItemIndicator.h"
#include "Item.h"

CUIItemIndicator::CUIItemIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject(pDevice, pContext)
{
}

CUIItemIndicator::CUIItemIndicator(const CUIItemIndicator& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIItemIndicator::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CUIItemIndicator::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CUIItemIndicator::Ready_Components()
{
    return S_OK;
}


CUIItemIndicator* CUIItemIndicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIItemIndicator* pInstance = new CUIItemIndicator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIItemIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIItemIndicator::Clone(void* pArg)
{
	CUIItemIndicator* pInstance = new CUIItemIndicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIItemIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIItemIndicator::Free()
{
	__super::Free();
}

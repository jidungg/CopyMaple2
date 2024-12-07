#include "stdafx.h"
#include "UIScrollButton.h"
#include "GameInstance.h"

CUIScrollButton::CUIScrollButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIButton(pDevice, pContext)
{
}

CUIScrollButton::CUIScrollButton(const CUIScrollButton& rhs)
	: CUIButton(rhs)
{
}


HRESULT CUIScrollButton::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    return S_OK;
}

void CUIScrollButton::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}



CUIScrollButton* CUIScrollButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIScrollButton* pInstance = new CUIScrollButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIScrollButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIScrollButton::Clone(void* pArg)
{
	CUIScrollButton* pInstance = new CUIScrollButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIScrollButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIScrollButton::Free()
{
	__super::Free();
}

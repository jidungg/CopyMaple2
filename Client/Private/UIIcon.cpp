#include "stdafx.h"
#include "UIIcon.h"

CUIIcon::CUIIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIPanel(pDevice, pContext)
{
}

CUIIcon::CUIIcon(const CUIIcon& Prototype)
	:CUIPanel(Prototype)
{
}

bool CUIIcon::Check_MouseOver(POINT fPos)
{
	return false;
}

CUIIcon* CUIIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIIcon* pInstance = new CUIIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIIcon");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIIcon::Clone(void* pArg)
{
	CUIIcon* pInstance = new CUIIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIIcon");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIIcon::Free()
{
	__super::Free();
}

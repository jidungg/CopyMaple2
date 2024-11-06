#include "stdafx.h"
#include "UIContainer.h"
#include "GameInstance.h"

CUIContainer::CUIContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject(pDevice, pContext)
{
}

CUIContainer::CUIContainer(const CUIContainer& Prototype)
	: CUIObject(Prototype)
{
}


bool CUIContainer::Check_MouseOver(POINT fPos)
{
	for (auto& i : m_pChilds)
	{
		if (static_cast<CUIObject*>(i)->Check_MouseOver(fPos))
			return true;
	}
	return false;
}

CUIContainer* CUIContainer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIContainer* pInstance = new CUIContainer(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUIContainer::Clone(void* pArg)
{
	CUIContainer* pInstance = new CUIContainer(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CUIContainer::Free()
{
	__super::Free();
}

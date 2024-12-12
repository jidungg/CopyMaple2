#include "stdafx.h"
#include "Canvas.h"

CCanvas::CCanvas()
	: CUIObject(nullptr,nullptr)
{
}

CCanvas::CCanvas(const CCanvas& rhs)
	: CUIObject(rhs)
{
}

HRESULT CCanvas::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCanvas::Initialize(void* pArg)
{
	return __super::Initialize(pArg);
}

HRESULT CCanvas::Render()
{
	return S_OK;
}


CCanvas* CCanvas::Create()
{
	CCanvas* pInstance = new CCanvas();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIPanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCanvas::Clone(void* pArg)
{
	CCanvas* pInstance = new CCanvas(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIPanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCanvas::Free()
{
	__super::Free();
}

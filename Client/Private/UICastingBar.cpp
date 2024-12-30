#include "stdafx.h"
#include "UICastingBar.h"
#include "GameInstance.h"

CUICastingBar::CUICastingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIBar(pDevice, pContext)
{
}

CUICastingBar::CUICastingBar(const CUIBar& Prototype)
	:CUIBar(Prototype)
{
}

void CUICastingBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

CUICastingBar* CUICastingBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUICastingBar* pInstance = new CUICastingBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUICastingBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUICastingBar::Clone(void* pArg)
{
	CUICastingBar* pInstance = new CUICastingBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUICastingBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUICastingBar::Free()
{
	__super::Free();
}

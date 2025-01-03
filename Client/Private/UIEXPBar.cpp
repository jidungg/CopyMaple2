#include "stdafx.h"
#include "UIEXPBar.h"
#include "UIVerticalFill.h"
#include "GameInstance.h"

CUIEXPBar::CUIEXPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CUIEXPBar::CUIEXPBar(const CUIEXPBar& Prototype)
	: CUIPanel(Prototype)
{
}

HRESULT CUIEXPBar::Initialize(void* pArg)
{
	UIEXPBAR_DESC* pDesc = static_cast<UIEXPBAR_DESC*>(pArg);
	m_pStat = pDesc->pStat;
	m_pDefaultStat = pDesc->pDefaultStat;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	CUIVerticalFill<_int>::UIVERTICALFILL_DESC tFillDesc;
	tFillDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tFillDesc.ePivotType = CORNOR_TYPE::CENTER;
	tFillDesc.fSizeX = pDesc->fSizeX;
	tFillDesc.fSizeY = pDesc->fSizeY;
	tFillDesc.fXOffset = 0;
	tFillDesc.fYOffset = 0;
	tFillDesc.vBorder = { 0,0,0,0 };
	tFillDesc.pValue = &(m_pStat->iEXP);
	tFillDesc.pDefaultValue = &(m_pDefaultStat->iEXP);
	tFillDesc.fVerticalEnd = { 1.f };
	tFillDesc.fVerticalStart = { 0.f };
	tFillDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, TEXT("uiexpbar_i6.dds")));
	tFillDesc.bVertical = false;
	m_pFill = static_cast<CUIVerticalFill<_int>*>(m_pGameInstance->Clone_Proto_Object_Stock(TEXT("Prototype_UI_IntVerticalFill"), &tFillDesc));
	if (nullptr == m_pFill)
		return E_FAIL;
	Add_Child(m_pFill);
	return S_OK;
}

void CUIEXPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);

}

CUIEXPBar* CUIEXPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIEXPBar* pInstance = new CUIEXPBar(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CUIEXPBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUIEXPBar::Clone(void* pArg)
{
	CUIEXPBar* pInstance = new CUIEXPBar(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CUIEXPBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIEXPBar::Free()
{
	__super::Free();
}

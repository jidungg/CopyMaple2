#include "stdafx.h"
#include "UIBar.h"
#include "Shader.h"
#include "GameInstance.h"

CUIBar::CUIBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}


CUIBar::CUIBar(const CUIBar& Prototype)
	: CUIPanel(Prototype)
{

}

HRESULT CUIBar::Initialize_Prototype()
{
	if (FAILED(CUIPanel::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CUIBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	UIBAR_DESC* pDesc = static_cast<UIBAR_DESC*>(pArg);
	m_fFillWidth = pDesc->fSizeX - pDesc->vFramePadding.z - pDesc->vFramePadding.w;
	m_fFillHeight = pDesc->fSizeY - pDesc->vFramePadding.x - pDesc->vFramePadding.y;

	CUIPanel::PANEL_DESC tFillDesc{};
	tFillDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFillDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tFillDesc.fSizeX = 1.f;
	tFillDesc.fSizeY = m_fFillHeight;
	tFillDesc.fXOffset = pDesc->vFramePadding.z;
	tFillDesc.fYOffset = pDesc->vFramePadding.x;
	tFillDesc.pTextureCom = pDesc->pFillTextureCom;
	tFillDesc.vBorder = pDesc->vFillBorder;
	m_pFillPanel = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &tFillDesc));
	Add_Child(m_pFillPanel);

	return S_OK;
}

void CUIBar::Update_Ratio(_float fRatio)
{
	m_fRatio = fRatio;
	static_cast<CRect_Transform*>(m_pFillPanel->Get_Transform())->Set_Size(m_fRatio * (m_fFillWidth), m_fFillHeight);
}

void CUIBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	//m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}



HRESULT CUIBar::Render()
{


	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

CUIBar* CUIBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIBar* pInstance = new CUIBar(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CUIBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUIBar::Clone(void* pArg)
{
	CUIBar* pInstance = new CUIBar(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CUIBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIBar::Free()
{
	__super::Free();
}

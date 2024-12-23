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
	m_fFrameXSize = static_cast<CRect_Transform*>(m_pTransformCom)->Get_ScreenSize().x;
	m_fFrameYSize = static_cast<CRect_Transform*>(m_pTransformCom)->Get_ScreenSize().y;
	UIBAR_DESC* pDesc = static_cast<UIBAR_DESC*>(pArg);
	m_pFillTextureCom = pDesc->pFillTextureCom;
	m_vFillBorder = pDesc->vFillBorder;

	m_pFillTransformCom = CRect_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pFillTransformCom)
		return E_FAIL;
	CRect_Transform::RECTTRANSFORM_DESC tFillDesc{};
	tFillDesc.eAnchorType = CORNOR_TYPE::LEFT;
	tFillDesc.ePivotType = CORNOR_TYPE::LEFT;
	tFillDesc.fSizeX = 1.f;
	tFillDesc.fSizeY = pDesc->fSizeY - pDesc->vBorder.x *2;
	tFillDesc.fXOffset = 3;
	tFillDesc.fYOffset = 0;
	m_pFillTransformCom->Set_Parent(m_pTransformCom);
	if (FAILED(m_pFillTransformCom->Initialize(&tFillDesc)))
		return E_FAIL;
	return S_OK;
}

void CUIBar::Update_Ratio(_float fRatio)
{
	m_fRatio = fRatio;
	m_pFillTransformCom->Set_Size(m_fRatio * (m_fFrameXSize - m_vBorder.z - m_vBorder.w), m_fFrameYSize - m_vBorder.y - m_vBorder.x);
}

void CUIBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}



HRESULT CUIBar::Render()
{


	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pFillTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pFillTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iSRVIndex)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_BorderSize", &m_vFillBorder, sizeof(XMUINT4))))
		return E_FAIL;
	_float4 vMinMax = static_cast<CRect_Transform*>(m_pFillTransformCom)->Get_MinMax();
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MinMax", &vMinMax, sizeof(XMUINT4))))
		return E_FAIL;
	if (m_pShaderCom)
		m_pShaderCom->Begin(0);
	if (m_pVIBufferCom)
	{
		m_pVIBufferCom->Bind_BufferDesc();
		m_pVIBufferCom->Render();
	}

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
	Safe_Release(m_pFillTextureCom);
	Safe_Release(m_pFillTransformCom);
}

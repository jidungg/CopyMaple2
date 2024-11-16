#include "stdafx.h"
#include "UISlot.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"

CUISlot::CUISlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)

{
}

CUISlot::CUISlot(const CUISlot& Prototype)
	: CUIPanel(Prototype)
	, m_pIconTexure(Prototype.m_pIconTexure)
	, m_pIconTransform(Prototype.m_pIconTransform)
{
	Safe_AddRef(m_pIconTexure);
	Safe_AddRef(m_pIconTransform);
}


HRESULT CUISlot::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUISlot::Initialize(void* pArg)
{
	if (FAILED(CUIPanel::Initialize(pArg)))
		return E_FAIL;

	UISLOT_DESC* pDesc = static_cast<UISLOT_DESC*>(pArg);
	CRect_Transform::RECTTRANSFORM_DESC tRectDesc;
	tRectDesc.eAnchorType = pDesc->tIconDesc.eAnchorType;
	tRectDesc.ePivotType = pDesc->tIconDesc.ePivotType;
	tRectDesc.fSizeX = pDesc->tIconDesc.fSizeX;
	tRectDesc.fSizeY = pDesc->tIconDesc.fSizeY;
	tRectDesc.fXOffset = pDesc->tIconDesc.fXOffset;
	tRectDesc.fYOffset = pDesc->tIconDesc.fYOffset;
	m_pIconTransform = static_cast<CRect_Transform*>(CRect_Transform::Create(m_pDevice, m_pContext));
	m_pIconTransform->Set_Parent(m_pTransformCom);
	m_pIconTransform->Initialize(&tRectDesc);

	if (pDesc->pTextureCom)
		m_pIconTexure = pDesc->pTextureCom;
    return S_OK;
}

HRESULT CUISlot::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_pShaderCom)
		m_pShaderCom->Begin(0);
	if (m_pVIBufferCom)
	{
		m_pVIBufferCom->Bind_BufferDesc();
		m_pVIBufferCom->Render();
	}
	if (Has_Item())
	{

		if (FAILED(m_pIconTransform->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;
		if (FAILED(m_pIconTexure->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
		_float4 vIconBorder = { 0,0,0,0 };
		if (FAILED(m_pShaderCom->Bind_RawValue("g_BorderSize", &vIconBorder, sizeof(XMUINT4))))
			return E_FAIL;
		_float4 vMinMax = static_cast<CRect_Transform*>(m_pIconTransform)->Get_MinMax();
		if (FAILED(m_pShaderCom->Bind_RawValue("g_MinMax", &vMinMax, sizeof(XMUINT4))))
			return E_FAIL;
		if (m_pShaderCom)
			m_pShaderCom->Begin(0);
		if (m_pVIBufferCom)
		{
			m_pVIBufferCom->Bind_BufferDesc();
			m_pVIBufferCom->Render();
		}
	}

	return S_OK;
}

void CUISlot::Set_IconTexture(CTexture* pTexture)
{
	Safe_Release(m_pIconTexure);
	m_pIconTexure = pTexture;
}

void CUISlot::Compute_Matrix()
{
	__super::Compute_Matrix();
	m_pIconTransform->Compute_Matrix();
}



void CUISlot::Free()
{
	__super::Free();
	Safe_Release(m_pIconTexure);
	Safe_Release(m_pIconTransform);
}

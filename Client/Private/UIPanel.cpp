#include "stdafx.h"
#include "UIPanel.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"

CUIPanel::CUIPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject(pDevice, pContext)
{
}

CUIPanel::CUIPanel(const CUIPanel& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIPanel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPanel::Initialize(void* pArg)
{
	PANEL_DESC* pDesc = static_cast<PANEL_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pDesc->eLevelID,pDesc->szTextureTag)))
		return E_FAIL;


	return S_OK;
}

void CUIPanel::Priority_Update(_float fTimeDelta)
{
}

HRESULT CUIPanel::Render_Self()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_BufferDesc();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUIPanel::Ready_Components(LEVELID eLevelID,const _tchar* szTextureTag)
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Add_Component(eLevelID, szTextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIPanel::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CUIPanel* CUIPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPanel* pInstance = new CUIPanel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIPanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIPanel::Clone(void* pArg)
{
	CUIPanel* pInstance = new CUIPanel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIPanel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPanel::Free()
{
	__super::Free();
}

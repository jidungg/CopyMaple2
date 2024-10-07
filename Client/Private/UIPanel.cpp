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

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUIPanel::Ready_Components()
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
	if(m_pTextureCom)
		if (FAILED(Add_Component(m_pTextureCom,TEXT("Com_Texture"))))
			return E_FAIL;

	return S_OK;
}
void CUIPanel::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
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

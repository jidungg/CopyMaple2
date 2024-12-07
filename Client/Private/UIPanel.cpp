#include "stdafx.h"
#include "UIPanel.h"
#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"

CUIPanel::CUIPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject(pDevice, pContext)
{
}

CUIPanel::CUIPanel(const CUIPanel& Prototype)
	: CUIObject(Prototype),
	m_pTextureCom{ Prototype.m_pTextureCom },
	m_pVIBufferCom{ Prototype.m_pVIBufferCom },
	m_pShaderCom{ Prototype.m_pShaderCom }
{

}

HRESULT CUIPanel::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CUIPanel::Initialize(void* pArg)
{
	PANEL_DESC* pDesc = static_cast<PANEL_DESC*>(pArg);
	if (pDesc->pTextureCom)
		m_pTextureCom = pDesc->pTextureCom;
	m_vBorder = pDesc->vBorder;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUIPanel::Render()
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

	for (auto& child : m_pChilds)
		if (child->Is_Active())
			child->Render();

	return S_OK;
}

HRESULT CUIPanel::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (m_pShaderCom)
	{
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_UI_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_UI_PROJ))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_BorderSize", &m_vBorder,sizeof(XMUINT4))))
			return E_FAIL;
		_float4 vMinMax = static_cast<CRect_Transform*>(m_pTransformCom)->Get_MinMax();
		if (FAILED(m_pShaderCom->Bind_RawValue("g_MinMax", &vMinMax, sizeof(XMUINT4))))
			return E_FAIL;
	}
	if (m_pTextureCom)
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iSRVIndex)))
			return E_FAIL;

	return S_OK;
}

HRESULT CUIPanel::Ready_Components()
{
	/* Com_Shader  그냥 셰이더 컴포넌트 만들기인데 만들고 나면 m_pShaderCom여기에 퐈인터가 저장돼야함*/
	//근데 nullptr임 보삼.
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_UI"),
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
void CUIPanel::Set_Texture(CTexture* pTexture)
{
	Remove_Component(m_pTextureCom);
	m_pTextureCom = pTexture;
	if (nullptr == pTexture)return;

	if (FAILED(Add_Component(m_pTextureCom, TEXT("Com_Texture"))))
		return ;
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

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

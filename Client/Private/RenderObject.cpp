#include "stdafx.h"
#include "RenderObject.h"

#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"

CRenderObject::CRenderObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CRenderObject::CRenderObject(const CRenderObject& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CRenderObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRenderObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;



	return S_OK;
}

HRESULT CRenderObject::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	m_pVIBufferCom = CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/FBXs/MAP/Cube/he_ground_grass_a01.fbx");

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("he_ground_grass_a01.dds"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}
HRESULT CRenderObject::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (m_pShaderCom)
	{
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
			return E_FAIL;
	}
	if (m_pTextureCom)
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;



	return S_OK;
}


void CRenderObject::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CRenderObject::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_pShaderCom)
		m_pShaderCom->Begin(0);
	if (m_pVIBufferCom)
		m_pVIBufferCom->Render();

	for (auto& child : m_pChilds)
		child->Render();

	return S_OK;
}



CRenderObject* CRenderObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderObject* pInstance = new CRenderObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRenderObject::Clone(void* pArg)
{
	CRenderObject* pInstance = new CRenderObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRenderObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CRenderObject::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

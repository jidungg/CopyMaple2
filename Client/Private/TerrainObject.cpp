#include "stdafx.h"
#include "TerrainObject.h"

#include "GameInstance.h"
#include "Mesh.h"
#include "Model.h"

CTerrainObject::CTerrainObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CTerrainObject::CTerrainObject(const CTerrainObject& Prototype)
	: CGameObject(Prototype),
	m_pTextureCom{ Prototype.m_pTextureCom },
	m_pVIBufferCom{ Prototype.m_pVIBufferCom },
	m_pShaderCom{ Prototype.m_pShaderCom }
{
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBufferCom);
	Safe_AddRef(m_pShaderCom);
}

HRESULT CTerrainObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrainObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;


	TERRAINOBJ_DESC* pDesc = (TERRAINOBJ_DESC*)pArg;

	pDesc->fRotationPerSec = 5.f;
	pDesc->fSpeedPerSec = 1.f;
	m_modleName = pDesc->modleName;
	m_eTerrObjType = pDesc->eType;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pDesc->Pos.x, pDesc->Pos.y, pDesc->Pos.z,1));
	m_pTransformCom->Rotation(pDesc->Rot);
	m_pTransformCom->Scaling(1, 1, 1);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTerrainObject::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Direction(XMVectorSet(0,-1,0,1), fTimeDelta);
}



json CTerrainObject::ToJson()
{
	json j;
	j["type"] = m_eTerrObjType;
	j["model"] = m_modleName.c_str();
	j["data"] = 0;
	j["rotation"] = { 0,0,0 };
	j["position"] = { 0,0,0 };
	j["iteration"] = 1;
	return j;
}

HRESULT CTerrainObject::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	m_pVIBufferCom = CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/FBXs/MAP/Cube/he_ground_grass_a01.fbx");
	Add_Component(m_pVIBufferCom, TEXT("Com_Buffer"));

	/* Com_Texture */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("he_ground_grass_a01.dds"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}
HRESULT CTerrainObject::Render()
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
HRESULT CTerrainObject::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CTerrainObject* CTerrainObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrainObject* pInstance = new CTerrainObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrainObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrainObject::Clone(void* pArg)
{
	CTerrainObject* pInstance = new CTerrainObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrainObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainObject::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

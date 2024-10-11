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
	m_pModelCom{ Prototype.m_pModelCom },
	m_pShaderCom{ Prototype.m_pShaderCom }
{
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pModelCom);
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
	m_eTerrainDir = pDesc->direction;
	m_iIndex = pDesc->index;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pDesc->pos.x, pDesc->pos.y, pDesc->pos.z,1));
	Rotate(m_eTerrainDir);
	m_pTransformCom->Scaling(1/(float)150, 1 / (float)150, 1 / (float)150);

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
	string str{ m_modleName.begin(), m_modleName.end() };
	j["model"] = str.c_str();
	j["data"] = 0;
	j["rotation"] = { 0,0,0 };
	j["position"] = { 0,0,0 };
	j["iteration"] = 1;
	return j;
}

void CTerrainObject::Turn(DIRECTION eDir)
{
}

void CTerrainObject::Rotate(DIRECTION eDir)
{
}

HRESULT CTerrainObject::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	wstring wstr = m_modleName;
	wstr += L".fbx";
	if (FAILED(Add_Component(LEVEL_LOADING, wstr,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;
	return S_OK; 
}
HRESULT CTerrainObject::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	//밖에서 반복하나ㅡㄴ 이유?
	for (size_t i = 0; i < iNumMeshes; i++)
	{

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			return E_FAIL;
		//패스 적용, 입력 레이아웃 Set
		m_pShaderCom->Begin(0);
		//정점,인덱스 버퍼 전달
		//그리기
		m_pModelCom->Render(i);
	}


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

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
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
	Safe_Release(m_pModelCom);
}

#include "stdafx.h"
#include "TerrainObject.h"

#include "GameInstance.h"
#include "Mesh.h"
#include "Model.h"
#include "MeshCollider.h"
#include "Client_Utility.h"

CTerrainObject::CTerrainObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModelObject(pDevice, pContext)
{
}

CTerrainObject::CTerrainObject(const CTerrainObject& Prototype)
	: CModelObject(Prototype),
	m_pColliderCom{ Prototype.m_pColliderCom }
{

	Safe_AddRef(m_pColliderCom);
}

HRESULT CTerrainObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrainObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	TERRAINOBJ_DESC* pDesc = (TERRAINOBJ_DESC*)pArg;

	pDesc->fRotationPerSec = 5.f;
	pDesc->fSpeedPerSec = 1.f;
	m_modleName = pDesc->wstrModelProtoName;
	m_eTerrObjType = pDesc->eType;
	m_eTerrainDir = pDesc->direction;
	m_iIndex = pDesc->index;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(pDesc->pos.x, pDesc->pos.y, pDesc->pos.z,1));
	
	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CTerrainObject::Update(_float fTimeDelta)
{
	m_pTransformCom->TurnToward(Get_Direction_Vector(m_eTerrainDir), fTimeDelta);
	__super::Update(fTimeDelta);
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

void CTerrainObject::Rotate()
{
	m_eTerrainDir = (DIRECTION)(((_uint)m_eTerrainDir + 1) % (_uint)DIRECTION::XMZP);

}




HRESULT CTerrainObject::Ready_Components(TERRAINOBJ_DESC* pDesc)
{
	CModelObject::Ready_Components(pDesc);

	//Com_Collider
	if (m_pModelCom->Get_NumMeshes() <= 0)
		m_pColliderCom = nullptr;
	else
	{
		CMeshCollider::MESH_COLLIDER_DESC desc{};
		desc.pMesh = m_pModelCom->Get_Mesh(0);
		if (FAILED(Add_Component(LEVEL_LOADING, CMeshCollider::m_szProtoTag,
			CCollider::m_szCompTag, reinterpret_cast<CComponent**>(&m_pColliderCom), &desc)))
			return E_FAIL;
	}

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
	Safe_Release(m_pColliderCom);
}

#include "stdafx.h"
#include "HumanModelObject.h"
#include "Face.h"
#include "GameInstance.h"

CHumanModelObject::CHumanModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModelObject(pDevice, pContext)
{

}

CHumanModelObject::CHumanModelObject(const CHumanModelObject& Prototype)
	: CModelObject(Prototype)
	, m_pFaceCom{ Prototype.m_pFaceCom }
{
	Safe_AddRef(m_pFaceCom);
}

HRESULT CHumanModelObject::Initialize_Prototype()
{
    return __super::Initialize_Prototype();;
}

HRESULT CHumanModelObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_mapMeshPartIdx[MESH_PART_ID::BRA] = m_pModelCom->Get_MeshIndex("CL_Bra_mesh0009");
	m_mapMeshPartIdx[MESH_PART_ID::TOP] = m_pModelCom->Get_MeshIndex("CL_Skin_mesh0001");
	m_mapMeshPartIdx[MESH_PART_ID::FACE] = m_pModelCom->Get_MeshIndex("FA_mesh0002");
	m_mapMeshPartIdx[MESH_PART_ID::EAR] = m_pModelCom->Get_MeshIndex("FA_EA_mesh0004");
	m_mapMeshPartIdx[MESH_PART_ID::HAIR] = m_pModelCom->Get_MeshIndex("HR_mesh0000");
	m_mapMeshPartIdx[MESH_PART_ID::PANTY] = m_pModelCom->Get_MeshIndex("PA_Panty_mesh0008");
	m_mapMeshPartIdx[MESH_PART_ID::BOTTOM] = m_pModelCom->Get_MeshIndex("PA_Skin_mesh0006");
	m_mapMeshPartIdx[MESH_PART_ID::GLOVE] = m_pModelCom->Get_MeshIndex("GL_mesh0005");
	m_mapMeshPartIdx[MESH_PART_ID::SHOES] = m_pModelCom->Get_MeshIndex("SH_mesh0007");
    return S_OK;
}
void CHumanModelObject::Set_MeshActive(MESH_PART_ID eID, _bool bIsOn)
{
	m_pModelCom->Set_MeshActive(m_mapMeshPartIdx[eID], bIsOn);
}
HRESULT CHumanModelObject::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);
	HUMANMODELOBJ_DESC* pDesc = (HUMANMODELOBJ_DESC*)pArg;

	//FACE
	m_pFaceCom = static_cast<CFace*>(m_pGameInstance->Clone_Proto_Component_Stock(pDesc->wstrFaceProtoTag));
	Add_Component(m_pFaceCom, TEXT("Com_Face"));

	//if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxHumanAnimMesh"),
	//	TEXT("Com_FaceShader"), reinterpret_cast<CComponent**>(&m_pFaceShaderCom))))
	//	return E_FAIL;
	return S_OK;
}

HRESULT CHumanModelObject::Render()
{
	if (FAILED(Bind_ShaderResources(m_pShaderCom)))
		return E_FAIL;
	//if (FAILED(Bind_ShaderResources(m_pFaceShaderCom)))
	//	return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	//m_pFaceCom->Bind_FaceShaderResource(m_pFaceShaderCom, "g_FaceTexture");
	m_pFaceCom->Bind_FaceShaderResource(m_pShaderCom, "g_FaceTexture");
	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if(false == m_pModelCom->Is_MeshActive(i))
			continue;

		//if (i == m_mapMeshPartIdx[MESH_PART_ID::FACE])
		//	pShader = m_pFaceShaderCom;
		//else
		//	pShader = m_pShaderCom;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEXTURE_TYPE::DIFFUSE, 0)))
			return E_FAIL;
		if (CModel::TYPE::TYPE_NONANIM != m_eModelType)
			if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
				return E_FAIL;
		if (i == m_mapMeshPartIdx[MESH_PART_ID::FACE])
			m_pShaderCom->Begin(1);
		else
			m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}


	for (auto& child : m_pChilds)
		child->Render();

	return S_OK;
}


CHumanModelObject* CHumanModelObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHumanModelObject* pInstance = new CHumanModelObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHumanModelObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHumanModelObject::Clone(void* pArg)
{
	CHumanModelObject* pInstance = new CHumanModelObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHumanModelObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanModelObject::Free()
{
	__super::Free();
	Safe_Release(m_pFaceCom);
	//Safe_Release(m_pFaceShaderCom);
}

#include "..\Public\Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CModel::CModel(const CModel& Prototype)
	: CComponent{ Prototype }
	, m_pAIScene{ Prototype.m_pAIScene }
	, m_iNumMeshes{ Prototype.m_iNumMeshes }
	, m_Meshes{ Prototype.m_Meshes }
	, m_iNumMaterials{ Prototype.m_iNumMaterials }
	, m_Materials{ Prototype.m_Materials }
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
	for (auto& mat : m_Materials)
		Safe_AddRef(mat);
}

HRESULT CModel::Initialize_Prototype(TYPE eModelType, const _char* pModelFilePath)
{
	_uint		iFlag = { aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast };

	if (eModelType == TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	if (0 == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	m_Meshes[iMeshIndex]->Bind_BufferDesc();
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType, _uint iTextureIndex)
{
	return m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()]->Bind_Texture(pShader, pConstantName, eType, iTextureIndex);
}

HRESULT CModel::Render()
{
	for (auto& mesh : m_Meshes)
	{
		mesh->Bind_BufferDesc();
		mesh->Render();
	}

	return S_OK;
}


HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_pAIScene->mMeshes[i]);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{

	m_iNumMaterials = m_pAIScene->mNumMaterials;

	m_Materials.resize(m_iNumMaterials);
	_char		szDrive[MAX_PATH] = "";
	_char		szDirectory[MAX_PATH] = "";
	_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
	strcat_s(szDrive, szDirectory);
	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, szDrive,m_pAIScene->mMaterials[i]);
		m_Materials[i] = pMaterial;
	}

	return S_OK;
}


CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const _char* pModelFilePath)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}
CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& mat : m_Materials)
		Safe_Release(mat);
	m_Materials.clear();
	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
	m_Meshes.clear();

	m_Importer.FreeScene();
}

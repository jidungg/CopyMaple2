#include "..\Public\Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Bone.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CModel::CModel(const CModel& Prototype)
	: CComponent{ Prototype }
	, m_eModelType{Prototype.m_eModelType }
	, m_iNumMeshes{ Prototype.m_iNumMeshes }
	, m_Meshes{ Prototype.m_Meshes }
	, m_iNumMaterials{ Prototype.m_iNumMaterials }
	, m_Materials{ Prototype.m_Materials }
	, m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
	for (auto& mat : m_Materials)
		Safe_AddRef(mat);
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath)
{

	std::ifstream inFile(pModelFilePath, std::ios::binary);
	if (!inFile) {
		string str = "파일을 열 수 없습니다.";
		str+= pModelFilePath;
		MessageBoxA(NULL, str.c_str(), "에러", MB_OK);
		return E_FAIL;
	}
	bool bAnim;
	inFile.read(reinterpret_cast<char*>(&bAnim), sizeof(bool));
	m_eModelType = bAnim ? TYPE_ANIM : TYPE_NONANIM;


	if (FAILED(Ready_Bones(inFile, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(inFile)))
		return E_FAIL;

	if (FAILED(Ready_Materials(inFile, pModelFilePath)))
		return E_FAIL;

	inFile.close();
	return S_OK;
}

HRESULT CModel::Ready_Meshes(ifstream& inFile)
{
	//inFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
	inFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
	cout  << m_iNumMeshes << endl;
	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType,this, inFile);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(ifstream& inFile, const _char* pModelFilePath)
{

	inFile.read(reinterpret_cast<char*>(&m_iNumMaterials), sizeof(_uint));
	cout << m_iNumMaterials << endl;
	m_Materials.resize(m_iNumMaterials);
	_char		szDrive[MAX_PATH] = "";
	_char		szDirectory[MAX_PATH] = "";
	_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
	strcat_s(szDrive, szDirectory);
	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, szDrive, inFile);
		m_Materials[i] = pMaterial;
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(ifstream& inFile, _int iParentBoneIndex)
{
	CBone* pBone = CBone::Create(inFile, iParentBoneIndex);

	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	iParentBoneIndex = m_Bones.size() - 1;
	_uint iNumChildren = 0;
	inFile.read(reinterpret_cast<char*>(&iNumChildren), sizeof(_uint));
	cout << iNumChildren << endl;
	for (size_t i = 0; i < iNumChildren; ++i)
	{
		Ready_Bones(inFile, iParentBoneIndex);
	}

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

HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEXTURE_TYPE eType, _uint iTextureIndex)
{
	return m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()]->Bind_Texture(pShader, pConstantName, eType, iTextureIndex);
}

void CModel::Play_Animation(_float fTimeDelta)
{
	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}
}

_uint CModel::Get_BoneIndex(const _char* pBoneName)
{
	_uint	iBoneIndex = { 0 };

	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (false == strcmp(pBone->Get_Name(), pBoneName))
				return true;

			++iBoneIndex;

			return false;
		});

	if (iter == m_Bones.end())
		MSG_BOX("그런 뼈가 없어");


	return iBoneIndex;
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




CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const _char* pModelFilePath)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath)))
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

}

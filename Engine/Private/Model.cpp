#include "..\Public\Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "MimicBone.h"
#include "GameInstance.h"
#include "Animation.h"

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
	, m_iNumAnimations{ Prototype.m_iNumAnimations }
{
	for (auto& pPrototypeBone : Prototype.m_Bones)
		m_Bones.push_back(pPrototypeBone->Clone());

	for (auto& pPrototypeAnimation : Prototype.m_Animations)
		m_Animations.push_back(pPrototypeAnimation->Clone());

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
	for (auto& mat : m_Materials)
		Safe_AddRef(mat);

}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

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

	if (FAILED(Ready_Animations(inFile)))
		return E_FAIL;
	inFile.close();
	return S_OK;
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	std::ifstream inFile(pModelFilePath, std::ios::binary);
	if (!inFile) {
		string str = "파일을 열 수 없습니다.";
		str += pModelFilePath;
		MessageBoxA(NULL, str.c_str(), "에러", MB_OK);
		return E_FAIL;
	}
	bool bAnim;
	inFile.read(reinterpret_cast<char*>(&bAnim), sizeof(bool));
	m_eModelType = eType;


	if (FAILED(Ready_Bones(inFile, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(inFile)))
		return E_FAIL;

	if (FAILED(Ready_Materials(inFile, pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations(inFile)))
		return E_FAIL;
	inFile.close();
	return S_OK;
}

HRESULT CModel::Ready_Bones(ifstream& inFile, _uint iParentBoneIndex)
{
	CBone* pBone;
	if(m_eModelType == TYPE::TYPE_MIMIC)
		pBone = CMimicBone::Create(inFile, iParentBoneIndex);
	else
		pBone = CBone::Create(inFile, iParentBoneIndex);

	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	iParentBoneIndex = (_uint)m_Bones.size() - 1;
	_uint iNumChildren = 0;
	inFile.read(reinterpret_cast<char*>(&iNumChildren), sizeof(_uint));
	//cout << iNumChildren << endl;
	for (size_t i = 0; i < iNumChildren; ++i)
	{
		Ready_Bones(inFile, iParentBoneIndex);
	}

	return S_OK;
}
HRESULT CModel::Ready_Meshes(ifstream& inFile)
{
	//inFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
	inFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
	//cout  << m_iNumMeshes << endl;
	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType,this, inFile, XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}
HRESULT CModel::Ready_Materials(ifstream& inFile, const _char* pModelFilePath)
{

	inFile.read(reinterpret_cast<char*>(&m_iNumMaterials), sizeof(_uint));
	//cout << m_iNumMaterials << endl;
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
HRESULT CModel::Ready_Animations(ifstream& inFile)
{
	inFile.read(reinterpret_cast<char*>(&m_iNumAnimations), sizeof(_uint));

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(inFile, this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation );
	}

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	if(m_eModelType == TYPE::TYPE_MIMIC)
	{
		MODEL_DESC* pDesc = static_cast<MODEL_DESC*>(pArg);
		const CModel* pTarget = pDesc->pMimicTarget;
		assert(pTarget != nullptr);
		for (auto& pBone : m_Bones)
		{
			static_cast<CMimicBone*>(pBone)->Set_Target(pTarget->Get_BoneMatrix(pBone->Get_Name()));
		}
	}
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

HRESULT CModel::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Bind_BoneMatrices(pShader, pConstantName, m_Bones);
}

bool CModel::Play_Animation(_float fTimeDelta)
{
	if (m_eModelType == TYPE::TYPE_MIMIC)
		return false;

	//뼈들의 변환행렬을 갱신
	bool bAnimEnd = false;
	if(m_iCurrentAnimIndex == m_iPrevAnimIndex)
		bAnimEnd = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, fTimeDelta);
	else
		if(m_Animations[m_iCurrentAnimIndex]->Update_AnimTransition(m_Bones, fTimeDelta, m_mapAnimTransLeftFrame))
			m_iPrevAnimIndex = m_iCurrentAnimIndex ;

	//뼈들의 합성변환행렬을 갱신
	for (auto& pBone : m_Bones)
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));

	return bAnimEnd;
}

_uint CModel::Get_MeshIndex(const _char* szName) const
{
	_uint	 iMeshIndex = { 0 };

	auto	iter = find_if(m_Meshes.begin(), m_Meshes.end(), [&](CMesh* pMesh)->_bool
		{
			if (false == strcmp(pMesh->Get_Name(), szName))
				return true;

			++iMeshIndex;

			return false;
		});

	if (iter == m_Meshes.end())
		MSG_BOX("그런 메쉬가 없어");


	return iMeshIndex;
}

_uint CModel::Get_BoneIndex(const _char* pBoneName) const
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

float CModel::Get_AnimTime()
{
	return m_Animations[m_iCurrentAnimIndex]->Get_AnimTime();
}

_uint CModel::Get_AnimIndex()
{
	return m_iCurrentAnimIndex;
}

const _float4x4* CModel::Get_BoneMatrix(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (false == strcmp(pBone->Get_Name(), pBoneName))
				return true;

			return false;
		});

	if (iter == m_Bones.end())
		return nullptr;

	return (*iter)->Get_CombinedTransformationFloat4x4();
}

CBone* CModel::Get_Bone(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (false == strcmp(pBone->Get_Name(), pBoneName))
				return true;

			return false;
		});

	if (iter == m_Bones.end())
		return nullptr;
	return *iter;
}

bool CModel::Is_AnimChangeable()
{
	return m_Animations[m_iCurrentAnimIndex]->Is_AnimChangeable();
}

bool CModel::Is_MeshActive(_uint iIdx)
{
	return m_Meshes[iIdx]->Is_Active();
}

void CModel::Set_AnimationLoop(_uint iIdx, _bool bIsLoop)
{
	m_Animations[iIdx]->Set_Loop(bIsLoop); 
}

void CModel::Set_Animation(_uint iIdx)
{
	m_iCurrentAnimIndex = iIdx;
	m_iPrevAnimIndex = iIdx;
	m_Animations[m_iCurrentAnimIndex]->Reset_CurrentTrackPosition();
}

void CModel::Set_AnimPostDelayPercent(_uint iIdx, _float fPercent)
{
	m_Animations[iIdx]->Set_PostDealyPercent(fPercent);
}

void CModel::Set_MeshActive(_uint iIdx, _bool bIsOn)
{
	m_Meshes[iIdx]->Set_Active(bIsOn);
}

void CModel::Switch_Animation(_uint iIdx)
{
	m_iPrevAnimIndex = m_iCurrentAnimIndex;
	m_iCurrentAnimIndex = iIdx;
	m_mapAnimTransLeftFrame.clear();
	m_Animations[m_iCurrentAnimIndex]->Reset_CurrentTrackPosition();
	m_Animations[m_iPrevAnimIndex]->Get_CurrentFrame( &m_mapAnimTransLeftFrame);
}



CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}
CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType,pModelFilePath, PreTransformMatrix)))
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
	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();
	for (auto& pAnim : m_Animations)
		Safe_Release(pAnim);
	m_Animations.clear();

}

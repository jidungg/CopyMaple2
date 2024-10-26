#include "MimicBoneModel.h"
#include "Bone.h"
#include "MimicBone.h"
#include "Mesh.h"

CMimicBoneModel::CMimicBoneModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModel(pDevice, pContext)
{
}

CMimicBoneModel::CMimicBoneModel(const CMimicBoneModel& Prototype)
	: CModel(Prototype)
{
}

HRESULT CMimicBoneModel::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	MIMICBONEMODEL_DESC* pDesc = static_cast<MIMICBONEMODEL_DESC*>(pArg);
	const CModel* pTarget = pDesc->pTarget;
	
	for (auto& pBone : m_Bones)
	{
		static_cast<CMimicBone*>( pBone)->Set_Target(pTarget->Get_BoneMatrix(pBone->Get_Name()));
	}
	//for (auto& mesh : m_Meshes)
	//{
	//	mesh->ReSet_OffsetMarix();
	//}
	return S_OK;
}

bool CMimicBoneModel::Play_Animation(_float fTimeDelta)
{
	//뼈들의 합성변환행렬을 갱신
	for (auto& pBone : m_Bones)
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));

	return false;
}

HRESULT CMimicBoneModel::Ready_Bones(ifstream& inFile, _uint iParentBoneIndex)
{
	CMimicBone* pBone = CMimicBone::Create(inFile, iParentBoneIndex);

	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	iParentBoneIndex = m_Bones.size() - 1;
	_uint iNumChildren = 0;
	inFile.read(reinterpret_cast<char*>(&iNumChildren), sizeof(_uint));
	//cout << iNumChildren << endl;
	for (size_t i = 0; i < iNumChildren; ++i)
	{
		Ready_Bones(inFile, iParentBoneIndex);
	}

	return S_OK;
}


CMimicBoneModel* CMimicBoneModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CMimicBoneModel* pInstance = new CMimicBoneModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMimicBoneModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMimicBoneModel* CMimicBoneModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CMimicBoneModel* pInstance = new CMimicBoneModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMimicBoneModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMimicBoneModel::Clone(void* pArg)
{
	CMimicBoneModel* pInstance = new CMimicBoneModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMimicBoneModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


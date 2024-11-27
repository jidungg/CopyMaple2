#include "..\Public\Bone.h"



CBone::CBone()
{
}


HRESULT CBone::Initialize(ifstream& inFile, _int iParentBoneIndex)
 {
	_uint iNameLength = 0;
	inFile.read((char*)&iNameLength, sizeof(_uint));
	//cout << iNameLength << endl;
	inFile.read((char*)&m_szName, iNameLength);
	m_szName[iNameLength] = '\0';
	//cout << m_szName << endl;
	//inFile.read((char*)&m_bBillboard, 1);
	inFile.read(reinterpret_cast<char*>(&m_TransformationMatrix), sizeof(_float4x4));
	//cout << m_TransformationMatrix._11 << ","<<m_TransformationMatrix._12 << "," << m_TransformationMatrix._13 << "," << m_TransformationMatrix._14 << endl;
	//cout << m_TransformationMatrix._21 << "," << m_TransformationMatrix._22 << "," << m_TransformationMatrix._23 << "," << m_TransformationMatrix._24 << endl;
	//cout << m_TransformationMatrix._31 << "," << m_TransformationMatrix._32 << "," << m_TransformationMatrix._33 << "," << m_TransformationMatrix._34 << endl;
	//cout << m_TransformationMatrix._41 << "," << m_TransformationMatrix._42 << "," << m_TransformationMatrix._43 << "," << m_TransformationMatrix._44 << endl;

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	XMStoreFloat4x4(&m_CombindTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	//루트 본 인 경우 -> PreTransformMatrix 만 곱하기
	if (-1 == m_iParentBoneIndex)
	{
		XMStoreFloat4x4(&m_CombindTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix );
	}
	//루트가 아닌 경우 -> 부모 본의 CombindTransformationMatrix 와 현재 본의 TransformationMatrix 를 곱하기
	else
	{
		XMStoreFloat4x4(&m_CombindTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) *
			XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombindTransformationMatrix));
	}
}

CBone* CBone::Create(ifstream& inFile, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(inFile, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}


void CBone::Free()
{
	__super::Free();
}

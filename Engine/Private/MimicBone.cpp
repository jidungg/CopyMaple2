#include "MimicBone.h"

CMimicBone::CMimicBone()
	: CBone()
{
}

void CMimicBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{	
	if (-1 == m_iParentBoneIndex)
	{
		XMStoreFloat4x4(&m_CombindTransformationMatrix, XMLoadFloat4x4(m_pCombindTransformationMatrix) * PreTransformMatrix);
	}
	else
		if(nullptr !=m_pCombindTransformationMatrix)
  			m_CombindTransformationMatrix = *m_pCombindTransformationMatrix;
	//auto mat = m_CombindTransformationMatrix;
	//cout << mat._11 << "," << mat._12 << "," << mat._13 << "," << mat._14 << endl;
	//cout << mat._21 << "," << mat._22 << "," << mat._23 << "," << mat._24 << endl;
	//cout << mat._31 << "," << mat._32 << "," << mat._33 << "," << mat._34 << endl;
	//cout << mat._41 << "," << mat._42 << "," << mat._43 << "," << mat._44 << endl;
	//cout << "==========================================================" << endl;
	//__super::Update_CombinedTransformationMatrix(Bones, PreTransformMatrix);
}



void CMimicBone::Set_Target(const _float4x4* pBoneMatrix)
{
	m_pCombindTransformationMatrix = pBoneMatrix;
}

CMimicBone* CMimicBone::Create(ifstream& inFile, _int iParentBoneIndex)
{
	CMimicBone* pInstance = new CMimicBone();

	if (FAILED(pInstance->Initialize(inFile, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CMimicBone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMimicBone* CMimicBone::Clone()
{
	return new CMimicBone(*this);
}

void CMimicBone::Free()
{
	__super::Free();
}

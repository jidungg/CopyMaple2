#include "MimicBone.h"

CMimicBone::CMimicBone()
	: CBone()
{
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

#pragma once
#include "Bone.h"

BEGIN(Engine)
class CMimicBone :
    public CBone
{

private:
	CMimicBone();
	virtual ~CMimicBone() = default;

public:

	virtual _matrix Get_CombinedTransformationMatrix() const override{
		if (m_pCombindTransformationMatrix == nullptr)
			return XMLoadFloat4x4(&m_CombindTransformationMatrix);
		else
			return XMLoadFloat4x4(m_pCombindTransformationMatrix);
	}
	virtual const _float4x4* Get_CombinedTransformationFloat4x4() const override {
		return m_pCombindTransformationMatrix;
	}

	void Set_Target(const _float4x4* pBone);
private:

	const _float4x4*			m_pCombindTransformationMatrix = {nullptr};


public:
	static CMimicBone* Create(ifstream& inFile, _int iParentBoneIndex);
	virtual CMimicBone* Clone();
	virtual void Free() override;
};

END
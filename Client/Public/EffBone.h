#pragma once
#include "Base.h"

BEGIN(Client)
class CEffBone :
    public CBase
{
protected:
	CEffBone();
	virtual ~CEffBone() = default;

public:
	HRESULT Initialize(ifstream& inFile, _int iParentBoneIndex);
	virtual void Update_CombinedTransformationMatrix(const vector<CEffBone*>& Bones, _fmatrix PreTransformMatrix);

public:
	const _char* Get_Name() const {
		return m_szName;
	}
	virtual _matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombindTransformationMatrix);
	}
	virtual const _float4x4* Get_CombinedTransformationFloat4x4() const {
		return &m_CombindTransformationMatrix;
	}
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}
protected:
	_char				m_szName[MAX_PATH] = {};

	_float4x4			m_TransformationMatrix = {};
	_float4x4			m_CombindTransformationMatrix = {};

	_int				m_iParentBoneIndex = { -1 };

	vector<_uint>		m_vecTrasnformControlIndex;

	_bool m_bBillboard = { false };
public:
	static CEffBone* Create(ifstream& inFile, _int iParentBoneIndex);
	virtual CEffBone* Clone();
	virtual void Free() override;
};

END
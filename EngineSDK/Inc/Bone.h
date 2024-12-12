#pragma once

#include "Base.h"

/* aiNode, aiBone, aiAnimNode */

BEGIN(Engine)

class CBone : public CBase
{
protected:
	CBone();
	virtual ~CBone() = default;

public:
	const _char* Get_Name() const {
		return m_szName;
	}

public:
	HRESULT Initialize(ifstream& inFile, _int iParentBoneIndex);
	virtual void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

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

	/* m_TransformationMatrix * Parent`s m_CombindTransformationMatrix  */
	_float4x4			m_CombindTransformationMatrix = {};

	_int				m_iParentBoneIndex = { -1 };

	_bool m_bBillboard = { false };
public:
	static CBone* Create(ifstream& inFile, _int iParentBoneIndex);
	virtual CBone* Clone();
	virtual void Free() override;
};

END
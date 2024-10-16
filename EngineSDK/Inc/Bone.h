#pragma once

#include "Base.h"

/* aiNode, aiBone, aiAnimNode */

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _char* Get_Name() const {
		return m_szName;
	}

public:
	HRESULT Initialize(ifstream& inFile, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

private:
	_char				m_szName[MAX_PATH] = {};

	_float4x4			m_TransformationMatrix = {};

	/* m_TransformationMatrix * Parent`s m_CombindTransformationMatrix  */
	_float4x4			m_CombindTransformationMatrix = {};

	_int				m_iParentBoneIndex = { -1 };

public:
	static CBone* Create(ifstream& inFile, _int iParentBoneIndex);
	virtual void Free() override;
};

END
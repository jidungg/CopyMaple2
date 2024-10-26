#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)
class CModel;
class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, CModel* pModel, ifstream& inFile, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	//셰이더에 본 행렬을 바인딩
	HRESULT Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, const vector<CBone*>& Bones);
	void ReSet_OffsetMarix();
public:
	HRESULT Ready_VertexBuffer_For_NonAnim(ifstream& inFile, _fmatrix PreTransformMatrix);
	HRESULT Ready_VertexBuffer_For_Anim(ifstream& inFile, class CModel* pModel);


private:
	_char						m_szName[MAX_PATH] = "";
	_uint						m_iMaterialIndex = {};
	_uint						m_iNumBones = {};

	vector<_uint>				m_BoneIndices;
	vector<_float4x4>			m_BoneOffsetMatrices;
	_float4x4					m_BoneMatrices[512];
public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType,  CModel* pModel, ifstream& inFile, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;


};

END
#pragma once
#include "VIBuffer.h"
BEGIN(Engine)
class CShader;

END
BEGIN(Client)
class CEffModel;
class CEffBone;
class CEffMesh : public CVIBuffer
{
private:
	CEffMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEffMesh() = default;
public:
	virtual HRESULT Initialize_Prototype(CEffModel* pModel, ifstream& inFile, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	//셰이더에 본 행렬을 바인딩
	HRESULT Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, const vector<CEffBone*>& Bones);
	void ReSet_OffsetMarix();
public:
	HRESULT Ready_VertexBuffer(ifstream& inFile, class CEffModel* pModel);
	HRESULT Ready_IndexBuffer(ifstream& inFile, class CEffModel* pModel, _uint iNumFaces);

	_uint Get_MaterialIndex() const {return m_iMaterialIndex;}
	_uint Get_TexturingIndex() const { return m_iTexturingIndex; }
	_char* Get_Name()  {return m_szName;}
private:
	_int						m_iMaterialIndex = {-1};
	//vector<_uint>      m_iMaterialControllerIndex;
	_int						m_iTexturingIndex = {-1};
	//vector<_uint>      m_iTextureTransformControllerIndex;

	_char						m_szName[MAX_PATH] = "";
	vector<_uint>				m_BoneIndices;
	vector<_float4x4>			m_BoneOffsetMatrices;
	_float4x4					m_BoneMatrices[512];
public:
	static CEffMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CEffModel* pModel, ifstream& inFile, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
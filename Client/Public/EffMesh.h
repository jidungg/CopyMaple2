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
	HRESULT Set_AlphaState();
	HRESULT Unset_AlphaState();
public:
	HRESULT Ready_VertexBuffer(ifstream& inFile, class CEffModel* pModel);
	HRESULT Ready_IndexBuffer(ifstream& inFile, class CEffModel* pModel, _uint iNumFaces);

	_uint Get_MaterialIndex() const {return m_iMaterialIndex;}
	_uint Get_TexturingIndex() const { return m_iTexturingIndex; }
	_char* Get_Name()  {return m_szName;}
protected:
	D3D11_BLEND_DESC Read_BlendDescFromFlags(_ushort& Flags);
	D3D11_BLEND Read_BlendModeFromFlags(_ushort& Flag);
	D3D11_DEPTH_STENCIL_DESC Read_TestDescFromFlags(_ushort& Flag);
	D3D11_COMPARISON_FUNC Read_TestModeFromFlags(_ushort& Flag);
private:
	_int						m_iMaterialIndex = {-1};
	//vector<_uint>      m_iMaterialControllerIndex;
	_int						m_iTexturingIndex = {-1};
	//vector<_uint>      m_iTextureTransformControllerIndex;

	_char						m_szName[MAX_PATH] = "";
	vector<_uint>				m_BoneIndices;
	vector<_float4x4>			m_BoneOffsetMatrices;
	_float4x4					m_BoneMatrices[512];
	_ushort 					m_AlphaFlags = { 0 };	

	D3D11_BLEND_DESC m_tBlendDesc;
	_float m_arrBlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	_uint m_iSampleMask = 0xffffffff;
	ID3D11BlendState* m_pBlendState = { nullptr };
	ID3D11BlendState* m_pOriginalBlendState ;
	_float m_arrOriginalBlendFactor[4] = { 1.f,1.f,1.f,1.f };
	_uint* m_pOriginalBlendMask ;

	D3D11_DEPTH_STENCIL_DESC m_tDepthStencilDesc;
	_float m_fTestThreshold = 0.0f;
	ID3D11DepthStencilState* m_pDepthStencilState = { nullptr };
	ID3D11DepthStencilState* m_pOriginalDepthStencilState = { nullptr };
public:
	static CEffMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CEffModel* pModel, ifstream& inFile, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
#pragma once
#include "Component.h"
BEGIN(Engine)
class CShader;
class CTexture;
END
BEGIN(Client)
typedef struct TextureTransformData
{
	_float2 f2Translation = { 0,0 };
	_float2 f2Scale = { 1,1 };
	_float fRotation = { 0 };
	_float2 f2Center = { 0.5,0.5 };
}TEX_TRANSFORM_DATA;
class CEffTextureSlot :
    public CComponent
{
protected:
	CEffTextureSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffTextureSlot(const CEffTextureSlot& Prototype);
    virtual ~CEffTextureSlot() = default;

public:
	HRESULT Initialize_Prototype(ifstream& inFIle);
	HRESULT Initialize(void* pArg);
	void Reset();
	const TextureTransformData& Get_TexTransformData() { return m_tTexTransformData; }
	void Set_TextureTransformData(EFF_TEX_OPERATION_TYPE eOpType, _float fValue);
	void Set_TextureIndex(_uint iTexIdx) { m_iTextureIndex = iTexIdx; }
	_uint Get_TextureIndex() { return m_iTextureIndex; }
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, vector<CTexture*>& vecTexture ,_uint iSRVIndex = 0);
private:
    _bool m_bHasTextureTransform = { false };
	_uint m_iTextureIndex = { UINT_MAX };
	_uint m_iDefaultTextureIndex = { UINT_MAX };
	TEX_TRANSFORM_DATA m_tTexTransformData;
	TEX_TRANSFORM_DATA m_tDefaultTexTransformData;

public:
	static CEffTextureSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,ifstream& inFIle);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
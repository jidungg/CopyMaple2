#pragma once
#include "Texture.h"
#include "EffControllable.h"
BEGIN(Engine)
class CShader;
END
BEGIN(Client)
typedef struct TextureTransformData
{
	_float2 f2Translation = { 0,0 };
	_float2 f2Scale = { 1,1 };
	_float fRotation = { 0 };
	_float2 f2Center = { 0.5,0.5 };
}TEX_TRANSFORM_DATA;
class CEffTexture :
    public CTexture
{
protected:
	CEffTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffTexture(const CEffTexture& Prototype);
    virtual ~CEffTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, EFF_TEX_TYPE iNumTexture);
	HRESULT Initialize_Prototype(const _char* szDirPath, ifstream& inFIle);
	void Reset();
	const TextureTransformData& Get_TexTransformData() { return m_tTexTransformData; }
	void Set_TextureTransformData(EFF_TEX_OPERATION_TYPE eOpType, _float fValue);
	ID3D11ShaderResourceView* Get_SRV() { return m_SRVs[0]; }
private:
    _bool m_bHasTextureTransform = { false };
	TEX_TRANSFORM_DATA m_tTexTransformData;
	TEX_TRANSFORM_DATA m_tDefaultTexTransformData;

public:
	static CEffTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, EFF_TEX_TYPE eTexType);
	static CEffTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, ifstream& inFIle, EFF_TEX_TYPE eTexType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once
#include "Texture.h"
BEGIN(Client)

class CEffTexture :
    public CTexture
{
protected:
	CEffTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffTexture(const CEffTexture& Prototype);
    virtual ~CEffTexture() = default;

public:
	HRESULT Initialize_Prototype(const _char* szDirPath, ifstream& inFIle);


private:
    _bool m_bHasTextureTransform = { false };
	_float2 m_f2Translation = { 0,0 };
	_float2 m_f2Scale = { 1,1 };
	_float m_fRotation = { 0 };
	_float2 m_f2Center = { 0.5,0.5 };
public:
	static CEffTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, ifstream& inFIle, EFF_TEX_TYPE eTexType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
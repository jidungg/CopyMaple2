#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture  : public CComponent
{
protected:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& Prototype);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar * pTextureFilePath, _uint iNumTexture);
	virtual HRESULT Initialize_Prototype(const _char* szDirPath, ifstream& inFIle);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, _uint iSRVIndex = 0);
	HRESULT Push_Texture(const _tchar* szPath);
	HRESULT Push_Texture(ID3D11ShaderResourceView* pSRV);
protected:
	_uint										m_iNumSRVs = { 0 };
	vector<ID3D11ShaderResourceView*>			m_SRVs;

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath,  ifstream& inFIle, TEXTURE_TYPE eTexType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END
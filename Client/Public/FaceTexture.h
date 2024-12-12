#pragma once
#include "Texture.h"

BEGIN(Client)
class CFaceTexture :
    public CTexture
{
private:
	explicit CFaceTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CFaceTexture(const CFaceTexture& Prototype);
	virtual ~CFaceTexture() = default;

public:
	HRESULT Initialize_Prototype(const _char* pTexFilePath);
	HRESULT Initialize(void* pArg)override;

private:
	_uint m_iFaceState = 0;
	_uint m_iFaceSubState = 0;

public:
	static CFaceTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pTexFilePath);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END 
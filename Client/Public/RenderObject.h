#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer;
class CTexture;
class CShader;
END	
BEGIN(Client)

class CRenderObject :
    public CGameObject
{
private:
	CRenderObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderObject(const CRenderObject& Prototype);
	virtual ~CRenderObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer* m_pVIBufferCom = { nullptr };
	_uint					m_iSRVIndex = 0;


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CRenderObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
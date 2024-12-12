#pragma once
#include "GameObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CWorldUIObject :
    public CGameObject
{
public:
	typedef struct WorldUIObjDesc
	{

	}WORLDUIOBJ_DESC;
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CWorldUIObject";
protected:
    explicit CWorldUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit CWorldUIObject(const CWorldUIObject& Prototype);
    virtual ~CWorldUIObject() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;
protected:
	virtual HRESULT Bind_ShaderResources(CShader* pShader);
    HRESULT Ready_Components(void* pArg);


protected:
    CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

protected:

public:
	static CWorldUIObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
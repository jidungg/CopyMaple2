#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)

class CShader;
class CTexture;
class CModel;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)
class CModelObject;
class CBuilder :
    public CGameObject
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Builder";
protected:
	explicit CBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBuilder(const CBuilder& Prototype);
	virtual ~CBuilder() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render();

private:
	CModelObject* m_pBird = nullptr;
public:
	static CBuilder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
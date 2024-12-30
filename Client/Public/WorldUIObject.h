#pragma once
#include "UIObject.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END
BEGIN(Client)
class CWorldUIObject :
    public CUIObject
{
public:
	typedef struct WorldUIObjDesc : public CUIObject::UIOBJECT_DESC
	{

	}WORLDUIOBJ_DESC;
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CWorldUIObject";
protected:
    explicit CWorldUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit CWorldUIObject(const CWorldUIObject& Prototype);
    virtual ~CWorldUIObject() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Compute_Matrix() override;
	
	void Set_WorldPosition(_fvector vPos) { m_vWorldPosition = vPos; }

protected:
	_vector m_vWorldPosition = { 0,0,0 ,1};
public:
	static CWorldUIObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
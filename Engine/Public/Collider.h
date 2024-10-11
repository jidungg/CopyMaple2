#pragma once
#include "Component.h"
BEGIN(Engine)
class CCollider :
    public CComponent
{
protected:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual void CheckCollision(CCollider* pOther) abstract;
	virtual void Render() {};

protected:
	_float3 m_vOffset = { 0.f, 0.f, 0.f };

public:
	virtual void Free() override;

};

END
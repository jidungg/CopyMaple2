#pragma once
#include "Collider.h"
BEGIN(Engine)
class CCollider_AABB :
    public CCollider
{
public:
	static constexpr _tchar m_szCompTag[] = L"Com_Collider_AABB";
protected:
	explicit CCollider_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCollider_AABB(const CCollider_AABB& Prototype);
	virtual ~CCollider_AABB() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual _bool Intersect( CCollider* pOther) override;
	virtual _bool RayCast(const Ray& tRay, RaycastHit* pOut) override;


protected:
	_float3 m_vMin = { -0.5f, -0.5f, -0.5f };
	_float3 m_vMax = { 0.5f, 0.5f, 0.5f };

public:
	static CCollider_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


	// CCollider을(를) 통해 상속됨
	void Update(_fmatrix WorldMatrix) override;

};

END
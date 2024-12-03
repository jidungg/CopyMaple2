#pragma once
#include "Collider.h"
BEGIN(Engine)

class ENGINE_DLL CCollider_Cylinder :
    public CColliderBase
{
public:
	typedef struct CylinderColliderDesc : public CColliderBase::ColliderDesc
	{
		_float		fHeight;
		_float		fRadius;
	}CYLINDER_COLLIDER_DESC;
	static constexpr _tchar m_szProtoTag[] = TEXT("Proto_Collider_Cylinder");

private:
	explicit CCollider_Cylinder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCollider_Cylinder(const CCollider_Cylinder& Prototype);
	virtual ~CCollider_Cylinder() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	// CCollider을(를) 통해 상속됨
	void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersects(CColliderBase* pOther) override;
	virtual _bool Contains(const FXMVECTOR& vPosition)override;
	;	virtual _bool RayCast(const Ray& tRay, RaycastHit* pOut) override;
	virtual HRESULT  Render() override;

	void Set_Radius(_float fRadius) { m_fRadius = fRadius; }
	void Set_Height(_float fHeight) { m_fHeight = fHeight; }
private:
	_float m_fHeight;
	_float m_fRadius;
	_vector m_vWorldCenter;
public:
	static CCollider_Cylinder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
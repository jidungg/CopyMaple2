#pragma once
#include "Collider.h"
BEGIN(Engine)
class ENGINE_DLL CCollider_Sphere :
    public CCollider
{
public:
	typedef struct SphereColliderDesc : public CCollider::ColliderDesc
	{
		_float		fRadius;
	}SPHERE_COLLIDER_DESC;
	static constexpr _tchar m_szProtoTag[] = TEXT("Proto_Collider_Sphere");
protected:
	explicit CCollider_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCollider_Sphere(const CCollider_Sphere& Prototype);
	virtual ~CCollider_Sphere() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	// CCollider을(를) 통해 상속됨
	void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersects(CCollider* pOther) override;
	virtual _bool Contains(const FXMVECTOR& vPosition)override;
;	virtual _bool RayCast(const Ray& tRay, RaycastHit* pOut) override;
	virtual HRESULT  Render() override;

	const BoundingSphere* Get_Desc() const {return m_pBoundDesc;}
	const _float3 Get_Offset() const { return m_pOriginalBoundDesc->Center; }
protected:
	BoundingSphere* m_pOriginalBoundDesc = {};
	BoundingSphere* m_pBoundDesc = {};
public:
	static CCollider_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END
#pragma once
#include "Collider.h"
BEGIN(Engine)
class ENGINE_DLL CCollider_AABB :
	public CCollider
{
public:
	typedef struct AABB_ColliderDesc : public CCollider::ColliderDesc
	{
		_float3		vExtentes;
	}AABB_COLLIDER_DESC;

	static constexpr _tchar m_szProtoTag[] = L"Prototype_Collider_AABB";
protected:
	explicit CCollider_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCollider_AABB(const CCollider_AABB& Prototype);
	virtual ~CCollider_AABB() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual _bool Intersect(CCollider* pOther) override;
	virtual _bool RayCast(const Ray& tRay, RaycastHit* pOut) override;
	virtual HRESULT  Render() override;
	BoundingBox* Get_Desc() { return m_pBoundDesc; }
protected:
	BoundingBox* m_pOriginalBoundDesc = {};
	BoundingBox* m_pBoundDesc = {};

public:
	static CCollider_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


	// CCollider을(를) 통해 상속됨
	void Update(_fmatrix WorldMatrix) override;

};

END
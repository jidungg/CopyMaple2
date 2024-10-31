#pragma once
#include "Collider.h"
BEGIN(Engine)
class ENGINE_DLL CCollider_Sphere :
    public CCollider
{
public:
	typedef struct : public CCollider::ColliderDesc
	{
		_float		fRadius;
	}SPHERE_COLLIDER_DESC;
	static constexpr _tchar m_szProtoTag[] = TEXT("Proto_Collider_Sphere");
protected:
	explicit CCollider_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCollider_Sphere(const CCollider_Sphere& Prototype);
	virtual ~CCollider_Sphere() = default;
	const BoundingSphere* Get_Desc() const {
		return m_pBoundDesc;
	}
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	// CCollider을(를) 통해 상속됨
	void Update(_fmatrix WorldMatrix) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual _bool Intersect(CCollider* pOther) override;
;	virtual _bool RayCast(const Ray& tRay, RaycastHit* pOut) override;
	virtual HRESULT  Render() override;

protected:
	BoundingSphere* m_pOriginalBoundDesc = {};
	BoundingSphere* m_pBoundDesc = {};
public:
	static CCollider_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END
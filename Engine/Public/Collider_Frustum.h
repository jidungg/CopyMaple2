#pragma once
#include "Collider.h"
BEGIN(Engine)
class ENGINE_DLL CCollider_Frustum :
    public CColliderBase
{
public:
	typedef struct Frutum_ColliderDesc : public CColliderBase::ColliderDesc
	{
		_float fDistance = 1.f;
		_float fWidth = 1.f;
		_float fHeight = 1.f;
		_float fEndWidth = 1.f;
	}FRUSTUM_COLLIDER_DESC;

	static constexpr _tchar m_szProtoTag[] = L"Prototype_Collider_Frustum";
private:
	explicit CCollider_Frustum(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCollider_Frustum(const CCollider_Frustum& Prototype);
	virtual ~CCollider_Frustum() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Initialize(_fmatrix matMatrix);
    void Update(_fmatrix WorldMatrix) override;
	virtual HRESULT  Render() override;

    _bool Intersects(CColliderBase* pOther) override;
    _bool Contains(const FXMVECTOR& vPos) override;
    _bool RayCast(const Ray& tRay, RaycastHit* pOut) override;
    BoundingFrustum* Get_Desc() { return m_pBoundDesc; }

private:
	BoundingFrustum* m_pOriginalBoundDesc = {};
	BoundingFrustum* m_pBoundDesc = {};
public:
	static CCollider_Frustum* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END
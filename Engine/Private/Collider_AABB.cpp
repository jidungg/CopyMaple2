#include "Collider_AABB.h"

CCollider_AABB::CCollider_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCollider(pDevice, pContext)
{
}

CCollider_AABB::CCollider_AABB(const CCollider_AABB& Prototype)
	:CCollider(Prototype)
{
}

HRESULT CCollider_AABB::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider_AABB::Initialize(void* pArg)
{
	return S_OK;
}
void CCollider_AABB::Update(_fmatrix WorldMatrix)
{
}

void CCollider_AABB::Late_Update(_float fTimeDelta)
{
}

_bool CCollider_AABB::Intersect(CCollider* pOther)
{
	return _bool();
}

_bool CCollider_AABB::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	return false;
}



CCollider_AABB* CCollider_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CComponent* CCollider_AABB::Clone(void* pArg)
{
	return nullptr;
}

void CCollider_AABB::Free()
{
	__super::Free();
}


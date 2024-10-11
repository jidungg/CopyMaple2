#pragma once
#include "Base.h"
BEGIN(Engine)
class CCollider;
class CPhysics :
    public CBase
{
public:
	static bool RayCast(_float3& vWorldOrigin, _float3& vWorldDir, CCollider* pOutTarget);
	static bool RayCast(Ray& tRay, CCollider* pOutTarget);
};

END
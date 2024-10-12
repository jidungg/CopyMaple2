#pragma once
#include "Base.h"

BEGIN(Engine)
class CCollider;
class CGameInstance;
class CPhysics :
    public CBase
{
private:
	CPhysics(CGameInstance* pGameInstance, _uint iWinSizeX, _uint iWinSizeY);
	virtual ~CPhysics() = default;
	HRESULT Initialize();
public:
	bool RayCast(const POINT& tScreenPos, CCollider* pCollider, RaycastHit* pOut);
	bool RayCast(const XMFLOAT4& vWorldOrigin, const XMFLOAT4& vWorldDir, CCollider* pCollider, RaycastHit* pOut);
	bool RayCast(const Ray& tRay, CCollider* pCollider, RaycastHit* pOut);


private:
	CGameInstance* m_pGameInstance = { nullptr };
	_uint m_iWinSizeX;
	_uint m_iWinSizeY;
public:
	static CPhysics* Create(CGameInstance* pGameInstance, _uint iWinSizeX, _uint iWinSizeY);
	virtual void Free() override;

};

END
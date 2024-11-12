#pragma once
#include "Base.h"

BEGIN(Engine)
class CColliderBase;
class CGameInstance;
class CPhysics :
    public CBase
{
private:
	CPhysics(CGameInstance* pGameInstance, _uint iWinSizeX, _uint iWinSizeY);
	virtual ~CPhysics() = default;
	HRESULT Initialize();
public:

private:
	CGameInstance* m_pGameInstance = { nullptr };
	_uint m_iWinSizeX;
	_uint m_iWinSizeY;
public:
	static CPhysics* Create(CGameInstance* pGameInstance, _uint iWinSizeX, _uint iWinSizeY);
	virtual void Free() override;

};

END
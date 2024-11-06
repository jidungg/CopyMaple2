#pragma once
#include "Base.h"
#include "Collider.h"

BEGIN(Engine)
class CObject_Manager;
class ENGINE_DLL CCollisionManager :
    public CBase
{
private:
	CCollisionManager(CObject_Manager* pObjMgr, _uint iLayerCount);
	virtual ~CCollisionManager() {}

public:
	HRESULT Initialize();
	void Set_LayerCount(_uint iLayerCount) { m_iLayerCount = iLayerCount; m_arrCheck.resize(m_iLayerCount, 0); }
	void Set_CollisionMatrix(_uint eObjectLayer, _uint eSubjectLayer, bool bValue);
	void Zero_CollisionMatrix();

	void Update_Collision();
	void Check_Collision_Layer(_uint eObjectLayer, _uint eSubjectLayer);

private:
	_uint m_iLayerCount = 0;
	vector<_uint> m_arrCheck;
	map<LONGLONG, bool> m_mapColBefore;
	CObject_Manager* m_pObjectMgr = nullptr;

public:
	static CCollisionManager* Create(CObject_Manager* pObjMgr, _uint iLayerCount);
	virtual void Free() override;
};
END

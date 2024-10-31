#pragma once
#include "Base.h"
#include "Collider.h"

BEGIN(Engine)
class ENGINE_DLL CCollisionMgr :
    public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	CCollisionMgr() : m_arrCheck{} {  }
	virtual ~CCollisionMgr() {}
private:
	virtual void Free();

public:
	void Set_CollisionMatrix(LAYER_ID eObjectLayer, LAYER_ID eSubjectLayer, bool bValue);
	void Update_Collision();
	void Zero_CollisionMatrix();

	void Check_Collision_Layer(LAYER_ID eObjectLayer, LAYER_ID eSubjectLayer);
	bool Is_Collision(CCollider* pObject, CCollider* pSubject, _vec3& vOutIntersect);
private:
	UINT m_arrCheck[(UINT)LAYER_ID::LAST];
	map<LONGLONG, bool> m_mapColBefore;
};
END

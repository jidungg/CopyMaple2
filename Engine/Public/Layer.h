#pragma once

#include "Base.h"

/* ��ü���� ���̾� ���� ��� �ټ� �����Ѵ�. */

BEGIN(Engine)
class CCollider;
class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	bool Check_Collision(const Ray& tRay, RaycastHit* pOut);
private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END
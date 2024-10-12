#pragma once

#include "Base.h"

/* 객체들을 레이어 별로 묶어서 다수 보관한다. */

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
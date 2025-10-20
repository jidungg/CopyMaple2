#pragma once

#include "Base.h"

/* ��ü���� ���̾� ���� ��� �ټ� �����Ѵ�. */

BEGIN(Engine)
class CColliderBase;
class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float fTimeDelta);
	//게임 로직
	void Update(_float fTimeDelta);
	//충돌처리
	void Late_Update(_float fTimeDelta);
	//죽은 오브젝트 처리
	void Final_Update();
	bool Check_Collision(const Ray& tRay, RaycastHit* pOut);
	vector<class CGameObject*>* Get_GameObjectList() { return &m_GameObjects; }
	void HandOver_DontDestroyObjects(list<class CGameObject*>* pOutDontDestroyLayer);
	CGameObject* Get_FirstGameObject();
private:
	vector<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END
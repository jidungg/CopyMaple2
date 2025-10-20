#pragma once

/* ���̾� : ���� ������ ���� �������� ������ ������ �׷�. */
/* ���̾���� �������� �����Ѵ�. */
#include "Base.h"

BEGIN(Engine)
class CGameObject;

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_GameObject_ToLayer(LEVEL_ID iPrototypeLevelIndex, const _wstring& strPrototypeTag, LEVEL_ID iLevelIndex, LAYER_ID iLayerId, void* pArg = nullptr, bool bDontDestroy = false);
	HRESULT Add_GameObject_ToLayer(LEVEL_ID iLevelIndex, LAYER_ID iLayerId, CGameObject* pObj, bool bDontDestroy = false);
	
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Final_Update();
	void Clear(LEVEL_ID iLevelIndex);
	void Move_DontDestroyObjects(LEVEL_ID iOldLevel, LEVEL_ID iNewLevel);
	bool RayCast(LAYER_ID iLayerId, const Ray& tRay, RaycastHit* pOut);
	bool RayCast(const Ray& tRay, RaycastHit* pOut);
	void Check_Collision(LAYER_ID iLayerId, CGameObject* pObject, vector<CGameObject*>* pOutList);

	CGameObject* Get_FirstGameObject(LEVEL_ID iLevIdx, LAYER_ID iLayerId);
	vector<CGameObject*>* Get_GameObjectList(LAYER_ID iLayerId);
private:
	_uint										m_iNumLevels = { 0 };
	typedef map<LAYER_ID, class CLayer*>	LAYERS;
	LAYERS*			m_pLayers = { nullptr };
	map<LEVEL_ID,list<class CGameObject*>>			m_DontDestroyLevel;

	class CGameInstance*						m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(LEVEL_ID iLevelIndex, LAYER_ID iLayerId);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END
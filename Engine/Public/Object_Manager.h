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
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, _uint iLayerId, void* pArg = nullptr, bool bDontDestroy = false);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, _uint iLayerId, CGameObject* pObj, bool bDontDestroy = false);
	
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Final_Update();
	void Clear(_uint iLevelIndex);
	void Move_DontDestroyObjects(_uint iOldLevel, _uint iNewLevel);
	bool RayCast(_uint iLayerId, const Ray& tRay, RaycastHit* pOut);
	bool RayCast(const Ray& tRay, RaycastHit* pOut);

	CGameObject* Get_FirstGameObject(_uint iLevIdx, _uint iLayerId);
	list<CGameObject*>* Get_GameObjectList(_uint iLayerId);
private:
	_uint										m_iNumLevels = { 0 };
	typedef map<_uint, class CLayer*>	LAYERS;
	LAYERS*			m_pLayers = { nullptr };
	map<_uint,list<class CGameObject*>>			m_DontDestroyLevel;

	class CGameInstance*						m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(_uint iLevelIndex, _uint iLayerId);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END
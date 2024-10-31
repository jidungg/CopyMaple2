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
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr, bool bDontDestroy = false);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* pObj, bool bDontDestroy = false);
	
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Final_Update();
	void Clear(_uint iLevelIndex);
	void On_OpenLevel(_uint iLevelIndex);
	bool RayCast(const _wstring& strLayerTag, const Ray& tRay, RaycastHit* pOut);
	bool RayCast(const Ray& tRay, RaycastHit* pOut);

	CGameObject* Get_FirstGameObject(_uint iLevIdx, const _wstring& strLayerTag);
private:
	_uint										m_iNumLevels = { 0 };
	typedef map<const _wstring, class CLayer*>	LAYERS;
	LAYERS*			m_pLayers = { nullptr };
	map<const _wstring ,list<class CGameObject*>>			m_DontDestroyLevel;

	class CGameInstance*						m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END
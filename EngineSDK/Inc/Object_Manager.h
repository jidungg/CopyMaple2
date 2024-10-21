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
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* pObj);

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Final_Update();
	void Clear(_uint iLevelIndex);
	bool RayCast(const _wstring& strLayerTag, const Ray& tRay, RaycastHit* pOut);
	bool RayCast(const Ray& tRay, RaycastHit* pOut);

private:
	_uint										m_iNumLevels = { 0 };
	map<const _wstring, class CLayer*>*			m_pLayers = { nullptr };
	typedef map<const _wstring, class CLayer*>	LAYERS;

	class CGameInstance*						m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END
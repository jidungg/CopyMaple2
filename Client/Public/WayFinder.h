#pragma once
#include "Component.h"
BEGIN(Client)
class CCubeTerrain;
class CTerrainObject;
class CWayFinder :
    public CComponent
{
protected:
	explicit CWayFinder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CWayFinder(const CWayFinder& Prototype);
	virtual ~CWayFinder() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	void MakeRoute(_uint iCurIdx, _uint iDestIdx);

private:
	_uint m_iSearchRange = { 10 };
	CCubeTerrain* m_pCubeTerrain = { nullptr };
	list<CTerrainObject*> m_OpenList;
	list<CTerrainObject*> m_CloseList;
public:
	static CWayFinder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
};

END 
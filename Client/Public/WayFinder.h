#pragma once
#include "Component.h"
BEGIN(Client)

class CCubeTerrain;
class CTerrainObject;
struct ASTARCELL
{
	_int iIdx = -1;
	_int iPIdx = -1;
	_float fG = 0;
	_float fH = 0;
	_float fF = 0;
};
class CWayFinder :
    public CComponent
{
public:
	typedef struct WayFinderDesc
	{
		CCubeTerrain* pCubeTerrain = { nullptr };
	}WAYFINDER_DESC;
protected:
	explicit CWayFinder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CWayFinder(const CWayFinder& Prototype);
	virtual ~CWayFinder() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	_bool	FindWay(_vector& vStart, _vector& vGoal, _uint iSearchRange);
	_vector Get_NextStation();
private:
	_bool MakeAStarRoute(_uint iStartIdx, _uint iDestIdx);

private:

	CCubeTerrain* m_pCubeTerrain = { nullptr };
	unordered_map<_int, ASTARCELL> m_mapAStarCell;
	list<_int> m_listOpenList;
	list<_int> m_listCloseList;
	list<_vector>		m_listRoute;
	list<_vector>::iterator m_iterRoute;
public:
	static CWayFinder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
};

END 
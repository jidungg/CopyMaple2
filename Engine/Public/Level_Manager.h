#pragma once

#include "Base.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Open_Level(_int iLevelIndex, class CLevel* pNewLevel);
	void Update(_float fTimeDelta);
	HRESULT Render();

	_int Get_CurrentLevelID() const { return m_iCurrentLevelID; }
	CLevel* Get_CurrentLevel() const { return m_pCurrentLevel; }
private:
	_int					m_iCurrentLevelID = { -1 };
	class CLevel*			m_pCurrentLevel = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END
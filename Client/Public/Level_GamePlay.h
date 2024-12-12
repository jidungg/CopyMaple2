#pragma once

#include "Level.h"

BEGIN(Client)
class CPlayer;
class CCubeTerrain;
class CLevel_GamePlay : public CLevel
{
public:
	typedef struct LevelDesc
	{
		_uint iPrevLevelID = { 0 };
		const _tchar* pCubeTerrainTag = { nullptr };
	}LEVEL_DESC;
protected:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;

	virtual void On_Start(_uint iPrevLevelID) override;
protected:
	CCubeTerrain* m_pCubeTerrain = { nullptr };
	CPlayer* m_pPlayer = { nullptr };
};

END
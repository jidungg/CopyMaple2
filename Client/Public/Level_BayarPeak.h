#pragma once
#include "Level_GamePlay.h"

BEGIN(Client)
class CLevel_BayarPeak :
    public CLevel_GamePlay
{
private:
	explicit CLevel_BayarPeak(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_BayarPeak() = default;

public:
	virtual void On_Start(_uint iPrevLevelID)override;


public:
	static CLevel_BayarPeak* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);

};

END
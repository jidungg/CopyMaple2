#pragma once
#include "Level_GamePlay.h"

BEGIN(Client)
class CLevel_HuntingPlace2 :
    public CLevel_GamePlay
{
private:
	explicit CLevel_HuntingPlace2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_HuntingPlace2() = default;

public:
	virtual void On_Start(_uint iPrevLevelID)override;


public:
	static CLevel_HuntingPlace2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);

};

END
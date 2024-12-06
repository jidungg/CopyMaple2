#pragma once
#include "Level_GamePlay.h"
BEGIN(Client)
class CLevel_Henesys :
    public CLevel_GamePlay
{
private:
	explicit CLevel_Henesys(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Henesys() = default;

public:
	virtual void On_Start(_uint iPrevLevelID)override;

	_float4x4 m_matTmp;
	_float4x4	 m_matTmpSocket;

public:
	static CLevel_Henesys* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
};

END
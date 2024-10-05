#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_Home final:
    public CLevel
{
private:
	explicit CLevel_Home(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Home() = default;

public:
	virtual HRESULT Initialize()override;
	virtual void Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);

public:
	static CLevel_Home* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

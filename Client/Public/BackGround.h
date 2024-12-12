#pragma once

#include "Client_Defines.h"
#include "UIPanel.h"


BEGIN(Client)

class CBackGround final : public CUIPanel
{
public:
	typedef struct BackgroundDesc: public CUIPanel::PANEL_DESC
	{

	}BACKGROUND_DESC;
private:
	CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackGround(const CBackGround& Prototype);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


public:
	static CBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END
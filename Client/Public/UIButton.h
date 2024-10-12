#pragma once

#include "Client_Defines.h"
#include "UIPanel.h"

BEGIN(Client)
class CUIButton :
    public CUIPanel
{
public:
	enum BUTTON_STATE { BS_NORMAL, BS_HIGHLIGHTED, BS_PRESSED, BS_DISABLED };
public:
	typedef struct ButtonDesc: public CUIPanel::PANEL_DESC
	{

	}BUTTON_DESC;

protected:
	CUIButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIButton(const CUIButton& Prototype);
	virtual ~CUIButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
private:
	HRESULT Ready_Components();

public:
	virtual void On_MouseOver();
	virtual void On_MouseEnter();
	virtual void On_MouseExit();
	virtual void On_MouseLButtonDown();
	virtual void On_MouseLButtonUp();
	virtual void On_MouseClick();

protected:

public:
	static CUIButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
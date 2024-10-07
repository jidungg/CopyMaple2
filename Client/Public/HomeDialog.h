#pragma once
#include "Client_Defines.h"
#include "UIPanel.h"
BEGIN(Client)
class CHomeDialog :
    public CUIPanel
{
public:
	typedef struct : public CUIPanel::PANEL_DESC
	{
	}HOMEDIALOG_DESC;

protected:
	CHomeDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHomeDialog(const CHomeDialog& Prototype);
	virtual ~CHomeDialog() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual void On_MouseEnter();
	virtual void On_MouseOver();
	virtual void On_MouseExit();
	virtual bool Consume_MouseLButtonDown();
	virtual bool Consume_MouseLButtonUp();
	virtual bool Consume_MouseRButtonDown();
	virtual bool Consume_MouseRButtonUp();
	virtual bool Consume_MouseClick();
private:
	HRESULT Ready_Childs();

public:
	static CHomeDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
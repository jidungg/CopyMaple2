#pragma once
#include "Client_Defines.h"
#include "UIListSelector.h"
#include "UIItemIndicator.h"

BEGIN(Client)

class CUIHomeDialog :
    public CUIPanel
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_HomeDialog";
public:
	typedef struct PanelDesc: public CUIPanel::PANEL_DESC
	{
		list<UIListItemData*>* listData;
	}HOMEDIALOG_DESC;

protected:
	CUIHomeDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIHomeDialog(const CUIHomeDialog& Prototype);
	virtual ~CUIHomeDialog() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;


	virtual void On_MouseEnter();
	virtual void MouseOver();
	virtual void On_MouseExit();
	virtual void On_MouseLButtonDown();
	virtual void On_MouseLButtonUp();
	virtual void On_MouseRButtonDown();
	virtual void On_MouseRButtonUp();
	virtual void On_MouseClick();

	void Register_OnClickCallback(const ButtonCallback& fCallback){m_pItemList->Register_OnClickCallback(fCallback);}

private:
	HRESULT Ready_Childs(HOMEDIALOG_DESC* pDesc);
	CUIListSelector* m_pItemList = nullptr;
public:
	static CUIHomeDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once

#include "Client_Defines.h"
#include "UIPanel.h"

BEGIN(Client)
using ButtonCallback = function<void(void*)>;

class CUIButton :
    public CUIPanel
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIButton";

public:
	enum BUTTON_STATE { BS_NORMAL, BS_HIGHLIGHTED, BS_PRESSED, BS_DISABLED, LAST };
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
	virtual void On_MouseOver()override;
	virtual void On_MouseEnter()override;
	virtual void On_MouseExit()override;
	virtual void On_MouseLButtonDown(const POINT& tMousePoint) override;
	virtual void On_MouseLButtonUp(const POINT& tMousePoint)override;
	virtual void On_MouseClick()override;
	virtual void On_MouseRightClick();

	void Register_OnClickCallback(const function<void(void*)>& fCallback) { m_listCallback.push_back(fCallback); }
	void Clear_OnClickCallback() { m_listCallback.clear(); }
	void Register_OnRightClickCallback(const function<void(void*)>& fCallback) { m_listRightClickCallback.push_back(fCallback); }
	void Clear_OnRightClickCallback() { m_listRightClickCallback.clear(); }

	void Set_Disable(_bool bValue);
	//void Remove_OnClickCallback(function<void(CBase&, void*)> fCallback)
	//{
	//	auto it = std::remove_if(m_listCallback.begin(), m_listCallback.end(), compareFunction);
	//	m_listCallback.erase(it);
	//}
protected:
	virtual void Call_Callback(const function<void(void*)>& fCallback);
	list<function<void(void*)> > m_listCallback;
	list<function<void(void*)> > m_listRightClickCallback;
	_bool m_bDisabled = { false };
	_uint m_arrSRVIndex[BUTTON_STATE::LAST] = { BS_NORMAL, BS_HIGHLIGHTED,BS_PRESSED,BS_DISABLED };
public:
	static CUIButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
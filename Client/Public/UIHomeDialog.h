#pragma once
#include "Client_Defines.h"
#include "UIListSelector.h"
#include "UIItemIndicator.h"

BEGIN(Client)
class CUIScroller;
class CUIHomeDialog :
    public CUIPanel
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_HomeDialog";
public:
	typedef struct PanelDesc: public CUIPanel::PANEL_DESC
	{
		_float fCommonPadding = 5;
		_float fItemHeight = 50;
		_float fItemWidth = 50;
		_uint iItemXCount = 10;
		_uint iItemYCount = 2;
		list<UIListItemData*>* listData;
	}HOMEDIALOG_DESC;

protected:
	CUIHomeDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIHomeDialog(const CUIHomeDialog& Prototype);
	virtual ~CUIHomeDialog() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual _uint AddRef() override;
	void Register_OnClickCallback(const ButtonCallback& fCallback){m_pItemList->Register_OnClickCallback(fCallback);}
	void Select_Item(_uint iID);
	void Select_NextItem();
private:
	HRESULT Ready_Childs(HOMEDIALOG_DESC* pDesc);
	CUIPanel* m_pItemBackPanel = { nullptr };
	CUIListSelector* m_pItemList = { nullptr };
	CUIScroller* m_pScroller = { nullptr };

	_float m_fCommonMargin = 5;
	_float m_fCommonButtonSize = 24;
	_float m_fItemHeight = 50;
	_float m_fItemWidth = 50;
	_uint m_iVisibleRowCount = 2;
	_uint m_iVisibleColCount = 10;
	_float m_fHeaderHeight = 40;
public:
	static CUIHomeDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
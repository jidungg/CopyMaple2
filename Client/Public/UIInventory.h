#pragma once
#include "UICommonWindow.h"
#include "UIInvenSlot.h"
#include "Item.h"

BEGIN(Client)
class CInventory;
class CUIListSelector;
class CUIScroller;
class CUIInvenTabButton;
class CUIInventory :
    public CUICommonWindow
{
public:
	typedef struct UIInventoryDesc : public CUICommonWindow::UICOMMONWINDOW_DESC
	{
		CInventory* pInventory = { nullptr };
	}UIINVENTORY_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIInventory";
private:
	explicit CUIInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIInventory(const CUIInventory& Prototype);
	virtual ~CUIInventory() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;

	virtual void Late_Update(_float fTimeDelta) override;
	HRESULT Ready_Slots();

	void Set_InventoryTab(ITEM_TYPE eType);
	void Update_Slot( _uint iIndex, ITEM_DATA* pData);
	void Clear_OnRightClickCallback();
private:
	CInventory* m_pInventory = { nullptr };
	_float2 m_fSlotSize = { 62,62 };
	_float2 m_fTabButtonSize = { 50,25 };
	_float2 m_fCommonMargin = { 5,5 };
	_float m_fHeaderHeight = { 55 };
	_uint m_iVisibleRowCount = { 8 };
	_uint m_iVisibleColCount = { 6 };
	ITEM_TYPE m_eCurrentTab = { ITEM_TYPE::EQUIP };

	CUIPanel* m_pItemBackPanel = { nullptr };
	CUIListSelector* m_pItemList = { nullptr };
	CUIScroller* m_pScroller = { nullptr };

	CUIInvenTabButton* m_arrTabButton[(_uint)ITEM_TYPE::LAST];
public:
	static CUIInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END
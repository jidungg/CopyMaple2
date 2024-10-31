#pragma once
#include "Base.h"
#include "Item.h"
#include "SlotItem.h"

BEGIN(Client)
class CInventory;
class CInvenSlot :
	public CBase, public ISlotItem
{
protected:
	CInvenSlot(ITEM_TYPE eItemType, _uint iIdx, CInventory* pInventory);
	virtual ~CInvenSlot() = default;

public:
	// ISlotItem을(를) 통해 상속됨
	virtual const _char* Get_IconTag() override;

	virtual HRESULT Insert_Item(ITEM_DESC* pData, _uint iCount = 1);
	virtual ITEM_DESC* Pop_Item(_uint iCount = 1);

	bool Is_Insertable(ITEM_DESC* pData, _uint iCount = 1);
	bool Is_Empty() { return m_pItemDesc == nullptr; }
	ITEM_DESC* Get_ItemDesc() { return m_pItemDesc; }
	_uint Get_Index()  { return m_iIndex; }
	ITEM_TYPE Get_Type()  { return m_eItemType; }

	void Set_ItemDesc(ITEM_DESC* pData) { m_pItemDesc = pData; }

	virtual void On_RightClick() abstract;
	virtual void On_LeftClick() abstract;

private:
	CInventory* m_pInventory = { nullptr };
	ITEM_TYPE m_eItemType = { ITEM_TYPE::LAST };
	ITEM_DESC* m_pItemDesc = { nullptr };
	_uint m_iStack = { 0 };
	_uint m_iMaxStack = { 0 };
	_uint m_iIndex = { 0 };
public:
	
	void Free();

};

END
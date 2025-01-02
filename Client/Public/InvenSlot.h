#pragma once
#include "Base.h"
#include "Item.h"
#include "SlotItem.h"

BEGIN(Client)
class CInventory;
class CInvenSlot :
	public CBase
{
protected:
	CInvenSlot(ITEM_TYPE eItemType, _uint iIdx, CInventory* pInventory);
	virtual ~CInvenSlot() = default;

public:
	// ISlotItem을(를) 통해 상속됨
	//virtual const _char* Get_IconTag() override;

	virtual HRESULT Insert_Item(const ITEM_DATA* pData, _uint iCount = 1);
	virtual const ITEM_DATA* Pop_Item(_uint iCount = 1);

	bool Is_Insertable(const ITEM_DATA* pData, _uint iCount = 1);
	bool Is_Empty() { return m_pItemDesc == nullptr; }
	const ITEM_DATA * Get_ItemData() { return m_pItemDesc; }
	_uint Get_StackCount() { return m_iStackCount; }

	void Set_ItemDesc(ITEM_DATA* pData) { m_pItemDesc = pData; }

	virtual void On_RightClick(void*) abstract;
	virtual void On_LeftClick(void*) abstract;



private:

	CInventory* m_pInventory = { nullptr };
	ITEM_TYPE m_eItemType = { ITEM_TYPE::LAST };
	const ITEM_DATA* m_pItemDesc = { nullptr };
	_uint m_iIndex = { 0 };
	_uint m_iStackCount = { 0 };
public:
	
	void Free();

};

END
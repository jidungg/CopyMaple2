#pragma once
#include "Base.h"
#include "Item.h"
#include "SlotItem.h"

BEGIN(Client)
class CPlayerEquipSlot
	: public CBase, public ISlotItem
{

protected:
	CPlayerEquipSlot(EQUIP_ITEM_TYPE eType);
	virtual ~CPlayerEquipSlot() = default;

public:
	// ISlotItem을(를) 통해 상속됨
	virtual const _char* Get_IconTag() override;

	HRESULT Insert_Item(const EQUIP_ITEM_DATA* pData);
	const EQUIP_ITEM_DATA* Pop_Item();



	const EQUIP_ITEM_DATA* Get_ItemDesc() { return m_pItemData; }
	bool Is_Empty() { return m_pItemData == nullptr; }
private:
	EQUIP_ITEM_TYPE m_eEquipType = { EQUIP_ITEM_TYPE::LAST };
	const EQUIP_ITEM_DATA* m_pItemData = { nullptr };
public:
	static CPlayerEquipSlot* Create(EQUIP_ITEM_TYPE eType);
	void Free();

};

END
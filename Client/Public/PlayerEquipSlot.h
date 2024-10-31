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

	HRESULT Insert_Item(EQUIP_ITEM_DESC* pData);
	EQUIP_ITEM_DESC* Pop_Item();



	EQUIP_ITEM_DESC* Get_ItemDesc() { return m_pItemDesc; }
	bool Is_Empty() { return m_pItemDesc == nullptr; }
private:
	EQUIP_ITEM_TYPE m_eEquipType = { EQUIP_ITEM_TYPE::LAST };
	EQUIP_ITEM_DESC* m_pItemDesc = { nullptr };
public:
	static CPlayerEquipSlot* Create(EQUIP_ITEM_TYPE eType);
	void Free();

};

END
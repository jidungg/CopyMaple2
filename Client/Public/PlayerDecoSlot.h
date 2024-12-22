#pragma once
#include "Base.h"
#include "Item.h"
#include "SlotItem.h"

BEGIN(Client)
class CPlayerDecoSlot :
    public CBase, public ISlotItem
{
protected:
	CPlayerDecoSlot(DECO_ITEM_TYPE eType);
	virtual ~CPlayerDecoSlot() = default;

public:
	// ISlotItem을(를) 통해 상속됨
	virtual const _char* Get_IconTag() override;
	virtual _bool Has_Item();

	HRESULT Insert_Item(const DECO_ITEM_DATA* pData);
	const DECO_ITEM_DATA* Pop_Item();



	const DECO_ITEM_DATA* Get_ItemDesc() { return m_pItemData; }
	bool Is_Empty() { return m_pItemData == nullptr; }
private:
	DECO_ITEM_TYPE m_eDecoType = { DECO_ITEM_TYPE::LAST };
	const DECO_ITEM_DATA* m_pItemData = { nullptr };
public:
	static CPlayerDecoSlot* Create(DECO_ITEM_TYPE eType);
	void Free();
};

END
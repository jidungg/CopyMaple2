#pragma once
#include "Base.h"
#include "Item.h"
#include "UIPlayerInfo.h"
BEGIN(Client)
class CPlayerInfoSlot :
    public CBase
{
protected:
	CPlayerInfoSlot(ITEM_TYPE eType);
	virtual ~CPlayerInfoSlot() = default;

public:
	HRESULT Insert_Item(const ITEM_DATA* pData);
	const ITEM_DATA* Pop_Item();

	virtual _bool Is_Insertable(const ITEM_DATA* pData);
	const ITEM_DATA* Get_ItemData() { return m_pItemData; }
	_bool Is_Empty() { return m_pItemData == nullptr; }


protected:
	CUIPlayerInfo::SLOT_ID m_eSlotID = { CUIPlayerInfo::SLOT_ID::SLOT_LAST };
private:
	ITEM_TYPE m_eItemType = {ITEM_TYPE::LAST };
	const ITEM_DATA* m_pItemData = { nullptr };
public:
	static CPlayerInfoSlot* Create(ITEM_TYPE eType);
	void Free();
};

END
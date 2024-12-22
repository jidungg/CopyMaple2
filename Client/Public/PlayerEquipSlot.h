#pragma once
#include "PlayerInfoSlot.h"
#include "Item.h"

BEGIN(Client)
class CPlayerEquipSlot
	: public CPlayerInfoSlot
{

protected:
	CPlayerEquipSlot(EQUIP_ITEM_TYPE eType);
	virtual ~CPlayerEquipSlot() = default;


private:

public:
	static CPlayerEquipSlot* Create(EQUIP_ITEM_TYPE eType);
	void Free();

};

END
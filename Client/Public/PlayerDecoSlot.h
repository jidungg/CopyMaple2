#pragma once
#include "PlayerInfoSlot.h"
#include "Item.h"

BEGIN(Client)
class CPlayerDecoSlot :
    public CPlayerInfoSlot
{
protected:
	CPlayerDecoSlot(DECO_ITEM_TYPE eType);
	virtual ~CPlayerDecoSlot() = default;

public:

public:
	static CPlayerDecoSlot* Create(DECO_ITEM_TYPE eType);
	void Free();
};

END
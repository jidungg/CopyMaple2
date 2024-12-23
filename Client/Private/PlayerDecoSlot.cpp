#include "stdafx.h"
#include "PlayerDecoSlot.h"

CPlayerDecoSlot::CPlayerDecoSlot(DECO_ITEM_TYPE eType)
	: CPlayerInfoSlot(ITEM_TYPE::DECO)
{
}

CPlayerDecoSlot* CPlayerDecoSlot::Create(DECO_ITEM_TYPE eType)
{
	return new CPlayerDecoSlot(eType);
}

void CPlayerDecoSlot::Free()
{
	__super::Free();
}

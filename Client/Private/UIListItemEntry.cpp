#include "stdafx.h"
#include "UIListItemEntry.h"
#include "UIList.h"

void IUIListItemEntry::On_CreateListItemEntry(CUIList* pList, _uint iIndex)
{
	m_iListItemIndex = iIndex;
}

#pragma once
#include "UIObject.h"
#include "Item.h"
BEGIN(Client)
class IUIListItemEntry
{
public:
	virtual HRESULT On_ListItemDataSet(const UIListItemData* data) = 0;
	_uint m_iListItemIndex = 0;
};
END
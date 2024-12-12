#pragma once
#include "Item.h"
BEGIN(Client)
class CUIList;
class IUIListItemEntry
{
public:
	virtual void On_CreateListItemEntry(CUIList* pList, _uint iIndex) ;
	virtual HRESULT On_ListItemDataSet(const UIListItemData* data) abstract;
	virtual void Set_ListItemEntryActive(_bool bActive) abstract;
	virtual HRESULT Render_ListEntry() abstract;

	_uint Get_ListItemIndex() { return m_iListItemIndex; }
private:
	_uint m_iListItemIndex = 0;
};
END
#pragma once
#include "Item.h"
BEGIN(Client)
class CUIList;
class IUIListItemEntry
{
public:
	virtual void On_CreateListItemEntry(CUIList* pList, _uint iIndex) ;
	virtual HRESULT On_ListItemDataSet(const ITEM_DATA* data) abstract;
	virtual void Set_ListItemEntryActive(_bool bActive) abstract;
	virtual HRESULT Render_ListEntry() abstract;
	virtual void Set_Offset(_float iX, _float iY) abstract;

	_uint Get_ListItemIndex() { return m_iListItemIndex; }
private:
	_uint m_iListItemIndex = 0;
};
END
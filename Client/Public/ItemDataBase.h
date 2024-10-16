#pragma once

#include "Base.h"
#include "Item.h"

BEGIN(Client)
class CItemDataBase :
    public CBase
{
	DECLARE_SINGLETON(CItemDataBase)
private:
	CItemDataBase();
	virtual ~CItemDataBase() = default;

public:
	HRESULT LoadFromJson();
	ITEM_DESC* GetItemDesc(ITEM_TYPE eType, wstring pKey) { return m_mapItem[(_uint)eType][pKey]; }
	map<wstring, ITEM_DESC*>* GetItemMap(ITEM_TYPE eType) { return &m_mapItem[(_uint)eType]; }
private:
	map<wstring , ITEM_DESC*> m_mapItem[(_uint)ITEM_TYPE::ITEM_TYPE_END];
public:
	void Free();
};
#define ITEMDB CItemDataBase::GetInstance()
END
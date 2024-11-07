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
	HRESULT Load_Data();
	void Insert_Data(ITEM_DATA* pData );
	ITEM_DATA* Get_Data(ITEM_TYPE eType, _uint iKey) { return m_mapItem[(_uint)eType][iKey]; }
	map<_uint, ITEM_DATA*>* GetItemMap(ITEM_TYPE eType) { return &m_mapItem[(_uint)eType]; }
private:
	map<_uint, ITEM_DATA*> m_mapItem[(_uint)ITEM_TYPE::LAST];
public:
	void Free();
};
#define ITEMDB CItemDataBase::GetInstance()
END
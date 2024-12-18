#pragma once
#include "InvenSlot.h"

BEGIN(Client)
class CInvenDecoSlot :
    public CInvenSlot
{
private:
	CInvenDecoSlot(_uint iIdx, CInventory* pInventory);
	virtual ~CInvenDecoSlot() = default;

public:
	virtual void On_RightClick(void*) override;
	virtual void On_LeftClick(void*) override;
	DECO_ITEM_TYPE Get_DecorType();
private:
	DECO_ITEM_TYPE m_eDecoType = { DECO_ITEM_TYPE::LAST };

public:
	static CInvenDecoSlot* Create(_uint iIdx, CInventory* pInventory);
	void Free();
};


class CInvenConsumableSlot :
	public CInvenSlot
{
private:
	CInvenConsumableSlot(_uint iIdx, CInventory* pInventory);
	virtual ~CInvenConsumableSlot() = default;

public:
	virtual void On_RightClick(void*) override;
	virtual void On_LeftClick(void*) override;
private:

	
public:
	static CInvenConsumableSlot* Create(_uint iIdx, CInventory* pInventory);
	void Free();
};


class CInvenBuildSlot :
	public CInvenSlot
{
private:
	CInvenBuildSlot(_uint iIdx, CInventory* pInventory);
	virtual ~CInvenBuildSlot() = default;

public:
	virtual void On_RightClick(void*) override;
	virtual void On_LeftClick(void*) override;
private:


public:
	static CInvenBuildSlot* Create(_uint iIdx, CInventory* pInventory);
	void Free();
};


class CInvenETCSlot :
	public CInvenSlot
{
private:
	CInvenETCSlot(_uint iIdx, CInventory* pInventory);
	virtual ~CInvenETCSlot() = default;

public:
	virtual void On_RightClick(void*) override;
	virtual void On_LeftClick(void*) override;
private:


public:
	static CInvenETCSlot* Create(_uint iIdx, CInventory* pInventory);
	void Free();
};
END
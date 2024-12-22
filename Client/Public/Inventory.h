#pragma once
#include "Base.h"
#include "Item.h"

BEGIN(Client)
class CInvenSlot;
class CInventory :
    public CBase
{
    DECLARE_SINGLETON(CInventory)
private:
	CInventory();
	~CInventory() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	HRESULT Insert_Item(const ITEM_DATA* pData, _uint iCount = 1);
	_bool Is_Insertable(const ITEM_DATA* pData, _uint iCount = 1);
	//HRESULT Remove_Item(ITEM_DESC* pData);

	vector<CInvenSlot*>* Get_Slots(ITEM_TYPE eItemType);
	
private:
	_uint m_iMaxSlot[(_uint)ITEM_TYPE::LAST] = { 48,48,96,96,96 };
	vector<CInvenSlot*> m_vecSlot[(_uint)ITEM_TYPE::LAST];
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	class CGameInstance* m_pGameInstance = { nullptr };

public:

	void Free();
};

#define INVENTORY CInventory::GetInstance()

END
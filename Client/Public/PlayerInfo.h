#pragma once
#include "Base.h"
#include "Item.h"

BEGIN(Client)
class CPlayer;
class CInventory;
class CInvenEquipSlot;
class CPlayerEquipSlot;
class CInvenDecoSlot;
class CPlayerDecoSlot;
class CPlayerInfo :
    public CBase
{
	DECLARE_SINGLETON(CPlayerInfo)
private:
	CPlayerInfo();
	virtual ~CPlayerInfo() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	HRESULT Equip(CInvenEquipSlot* pInvenSlot);
	HRESULT UnEquip(EQUIP_ITEM_TYPE eType);
	HRESULT Equip(CInvenDecoSlot* pInvenSlot);
	HRESULT UnEquip(DECO_ITEM_TYPE eType);
	void Toggle_UI();
	CPlayer* Get_Player() { return m_pPlayer; }

private:
	CPlayerEquipSlot* m_pEquipSlots[(_uint)EQUIP_ITEM_TYPE::LAST] = { nullptr, };
	class CUIPlayerInfo* m_pPlayerInfoUI = { nullptr };
	CPlayer* m_pPlayer = { nullptr };
	CInventory* m_pInventory = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
public:
	void Free();
};

#define PLAYERINIFO CPlayerInfo::GetInstance()

END
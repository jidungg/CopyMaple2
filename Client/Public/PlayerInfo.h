#pragma once
#include "Base.h"
#include "Item.h"
#include "UIPlayerInfoSlot.h"

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
public:

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
	HRESULT Gain_Item(const ITEM_DATA* pItem, _uint iCount = 1);
	HRESULT Gain_Item(ITEM_TYPE eITemType, _uint iItemId, _uint iCount = 1);
	void Gain_EXP(_int iAmount);
	void Gain_Gold(_int iAmount);
	_bool Is_Insertable(const ITEM_DATA* pItem, _uint iCount = 1);
	CPlayerInfoSlot* Get_Slot(CUIPlayerInfo::SLOT_ID eSlotID);


	_int Get_HP();
	_int Get_SP();
	_int Get_EP();

	_int Get_TotalSTR();
	_int Get_TotalDEX();
	_int Get_TotalINT();
	_int Get_TotalLUK();
	_int Get_TotalAttack();
	_int Get_TotalHP();
	_float Get_TotalCRIT();
	_int Get_TotalDefense();

	_int Get_AttackPoint();
	_int Get_DefensePoint();
	_int Get_BattlePoint();

	_int Get_LeftStatPoint() { return m_iLeftStatPoint; }

	void Increase_Stat(_uint iStatType);



private:
	CPlayerEquipSlot* m_pEquipSlots[(_uint)EQUIP_ITEM_TYPE::LAST] = { nullptr, };
	CPlayerDecoSlot* m_pDecoSlots[(_uint)DECO_ITEM_TYPE::LAST] = { nullptr, };
	class CUIMainHUDGuage* m_pPlayerInfoUI = { nullptr };
	CPlayer* m_pPlayer = { nullptr };
	CInventory* m_pInventory = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
	_uint m_iLeftStatPoint = { 0 };
public:
	void Free();
};

#define PLAYERINIFO CPlayerInfo::GetInstance()

END
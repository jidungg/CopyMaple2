#include "stdafx.h"
#include "PlayerInfo.h"
#include "Player.h"
#include "Inventory.h"
#include "PlayerEquipSlot.h"
#include "InvenEquipSlot.h"
#include "GameInstance.h"
#include "InvenDecoSlot.h"
#include "PlayerDecoSlot.h"
#include "WorldItem.h"
#include "Client_Utility.h"
#include "ItemDataBase.h"
#include "Status.h"

IMPLEMENT_SINGLETON(CPlayerInfo)
CPlayerInfo::CPlayerInfo()
	: CBase()
	, m_pInventory(INVENTORY)
	, m_pGameInstance(CGameInstance::GetInstance())
{
}

HRESULT CPlayerInfo::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	for (_uint iEqType = 0; iEqType < (_uint)EQUIP_ITEM_TYPE::LAST; iEqType++)
		m_pEquipSlots[iEqType] = CPlayerEquipSlot::Create((EQUIP_ITEM_TYPE)iEqType);
	for (_uint iDecoType = 0; iDecoType < (_uint)DECO_ITEM_TYPE::LAST; iDecoType++)
		m_pDecoSlots[iDecoType] = CPlayerDecoSlot::Create((DECO_ITEM_TYPE)iDecoType);

	CPlayer::PLAYER_DESC		PlayerDesc{};

	PlayerDesc.fSpeedPerSec = 5.f;
	PlayerDesc.fRotationPerSec = XMConvertToRadians(90.f);
	json jPlayerData;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/PlayerData.json", &jPlayerData)))
		return E_FAIL;

	PlayerDesc.jPlayerData = jPlayerData["Player"];
	m_pPlayer = static_cast<CPlayer*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, TEXT("Prototype_GameObject_Player"), &PlayerDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOGO, LAYER_PLAYER, m_pPlayer, true)))
		return E_FAIL;
	Safe_AddRef(m_pPlayer);
	m_iLeftStatPoint = m_pPlayer->Get_Stat().iLEVEL * 5;

	return S_OK;
}



HRESULT CPlayerInfo::Equip(CInvenEquipSlot* pInvenSlot)
{
	const EQUIP_ITEM_DATA* pEQDesc = static_cast<const EQUIP_ITEM_DATA	*>( pInvenSlot->Get_ItemData());
	EQUIP_ITEM_TYPE  eEQType = pEQDesc->eEquipType;
	assert(pEQDesc != nullptr);

	//해당 부위에 장착된 아이템이 장착 해제. 
	//한벌은 SUIT, TOP, BOTTOM 모두 장착 해제해야함.
	list <const ITEM_DATA*> pPopedItem;
	pPopedItem.push_back(m_pEquipSlots[(_uint)eEQType]->Pop_Item());
	m_pPlayer->UnEquip(eEQType);
	if (eEQType == EQUIP_ITEM_TYPE::SUIT)
	{
		pPopedItem.push_back(m_pEquipSlots[(_uint)EQUIP_ITEM_TYPE::TOP]->Pop_Item());
		pPopedItem.push_back(m_pEquipSlots[(_uint)EQUIP_ITEM_TYPE::BOTTOM]->Pop_Item());
		m_pPlayer->UnEquip(EQUIP_ITEM_TYPE::TOP);
		m_pPlayer->UnEquip(EQUIP_ITEM_TYPE::BOTTOM);
	}
	else if (eEQType == EQUIP_ITEM_TYPE::TOP)
	{
		pPopedItem.push_back(m_pEquipSlots[(_uint)EQUIP_ITEM_TYPE::SUIT]->Pop_Item());
		m_pPlayer->UnEquip(EQUIP_ITEM_TYPE::SUIT);
	}
	else if(eEQType == EQUIP_ITEM_TYPE::BOTTOM)
	{
		pPopedItem.push_back(m_pEquipSlots[(_uint)EQUIP_ITEM_TYPE::SUIT]->Pop_Item());
		m_pPlayer->UnEquip(EQUIP_ITEM_TYPE::SUIT);
	}

	m_pEquipSlots[(_uint)eEQType]->Insert_Item(pEQDesc);
	m_pPlayer->Equip(pEQDesc);
	pInvenSlot->Pop_Item();

	for (auto& pPoped : pPopedItem)
	{
		if (pPoped != nullptr)
			if(FAILED(m_pInventory->Insert_Item(pPoped)))
			{
		//TODO : 밖으로 템 떨구기.
				MSG_BOX("인벤토리 공간이 부족합니다.");
				return E_FAIL;
			}
	}
	m_pEquipSlots[(_uint)eEQType]->Insert_Item(pEQDesc);
	return S_OK;
}

HRESULT CPlayerInfo::UnEquip(EQUIP_ITEM_TYPE eType)
{
	//TODO : CUIPlayerInfoEquipSlot 에서 우클릭 시 호출
	//먼저 인벤에 빈 공간이 있는지 확인
	//있으면 인벤에 넣고, 플레이어에서 장착 해제
	//없으면 밖으로 떨구기.
	const ITEM_DATA* pItem = m_pEquipSlots[(_uint)eType]->Pop_Item();
	if (pItem == nullptr)
		return E_FAIL;
	HRESULT hr =  m_pInventory->Insert_Item(pItem);
	if (FAILED(hr))
	{
		CWorldItem::WORLDITEM_DESC tItemDesc;
		tItemDesc.pItemData = pItem;
		tItemDesc.iStackCount = 1;
		CWorldItem* pItem = static_cast<CWorldItem*>(m_pGameInstance->Clone_Proto_Object_Stock(CWorldItem::m_szProtoTag, &tItemDesc));
		pItem->Set_Transform(m_pPlayer->Get_WorldPosition());
		pItem->Compute_Matrix();
		m_pGameInstance->Add_GameObject_ToLayer((_uint)Get_CurrentTrueLevel(), (_uint)LAYERID::LAYER_WORLD_ITEM, pItem);
		pItem->PopUp();
	}
	m_pPlayer->UnEquip(eType);

	return S_OK;
}

HRESULT CPlayerInfo::Equip(CInvenDecoSlot* pInvenSlot)
{
	const DECO_ITEM_DATA* pData = static_cast<const DecoItemData*>( pInvenSlot->Get_ItemData());
	const DECO_ITEM_DATA* pPoped = static_cast<const DecoItemData*>( m_pDecoSlots[(_uint)pData->eDecoType]->Pop_Item());
	m_pDecoSlots[(_uint)pData->eDecoType]->Insert_Item(pData);
	m_pPlayer->Equip(pData);
	pInvenSlot->Pop_Item();
	if (pPoped != nullptr)
	{
		if (FAILED(m_pInventory->Insert_Item(pPoped)))
		{
			//TODO : 밖으로 템 떨구기.
			MSG_BOX("인벤토리 공간이 부족합니다.");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CPlayerInfo::UnEquip(DECO_ITEM_TYPE eType)
{
	const ITEM_DATA* pItem = m_pDecoSlots[(_uint)eType]->Pop_Item();
	HRESULT hr = m_pInventory->Insert_Item(pItem);
	if (FAILED(hr))
	{
		CWorldItem::WORLDITEM_DESC tItemDesc;
		tItemDesc.pItemData = pItem;
		tItemDesc.iStackCount = 1;
		CWorldItem* pItem = static_cast<CWorldItem*>(m_pGameInstance->Clone_Proto_Object_Stock(CWorldItem::m_szProtoTag, &tItemDesc));
		pItem->Set_Transform(m_pPlayer->Get_WorldPosition());
		pItem->Compute_Matrix();
		m_pGameInstance->Add_GameObject_ToLayer((_uint)Get_CurrentTrueLevel(), (_uint)LAYERID::LAYER_WORLD_ITEM, pItem);
		pItem->PopUp();
	}
	m_pPlayer->UnEquip(eType);

	return S_OK;
}

void CPlayerInfo::Toggle_UI()
{
}

HRESULT CPlayerInfo::Gain_Item(const ITEM_DATA* pItem, _uint iCount)
{
	return m_pInventory->Insert_Item(pItem,iCount);
}

HRESULT CPlayerInfo::Gain_Item(ITEM_TYPE eITemType, _uint iItemId, _uint iCount)
{
	return m_pInventory->Insert_Item(ITEMDB->Get_Data(eITemType, iItemId), iCount);
}

void CPlayerInfo::Gain_EXP(_int iAmount)
{
	Stat* pStat =  m_pPlayer->Get_Stat_Ref();
	Stat* pDefaultStat =  m_pPlayer->Get_DefaultStat_Ref();

	pStat->iEXP += iAmount;
	pDefaultStat->iEXP = pStat->iLEVEL * pStat->iLEVEL * 10;
	while (pDefaultStat->iEXP <= pStat->iEXP)
	{
		pStat->iLEVEL++;
		pStat->iEXP -= pDefaultStat->iEXP;
		pDefaultStat->iEXP = pStat->iLEVEL * pStat->iLEVEL * 10;
		m_pPlayer->FullRecovery();
		m_iLeftStatPoint += 5;
	}
}

void CPlayerInfo::Gain_Gold(_int iAmount)
{
	return m_pInventory->Gain_Gold(iAmount);
}

_bool CPlayerInfo::Is_Insertable(const ITEM_DATA* pItem, _uint iCount)
{
	return m_pInventory->Is_Insertable(pItem,iCount);
}

CPlayerInfoSlot* CPlayerInfo::Get_Slot(CUIPlayerInfo::SLOT_ID eSlotID)
{
	return nullptr;
}

_int CPlayerInfo::Get_HP()
{
	return m_pPlayer->Get_Stat().iHP;
}

_int CPlayerInfo::Get_SP()
{
	return m_pPlayer->Get_Stat().iSP;
}

_int CPlayerInfo::Get_EP()
{
	return m_pPlayer->Get_Stat().iEP;
}

_int CPlayerInfo::Get_TotalSTR()
{
	_int iSTR = m_pPlayer->Get_Stat().iSTR;
	for (auto& pEquipSlot : m_pEquipSlots)
	{
		const EquipItemData* pData = static_cast<const EquipItemData*>(pEquipSlot->Get_ItemData());
		if (pData)
			iSTR += pData->iSTR ;
	}
	return iSTR;
}

_int CPlayerInfo::Get_TotalDEX()
{
	_int iDEX = m_pPlayer->Get_Stat().iDEX;
	for (auto& pEquipSlot : m_pEquipSlots)
	{
		const EquipItemData* pData = static_cast<const EquipItemData*>(pEquipSlot->Get_ItemData());
		if (pData)
			iDEX += pData->iDEX;
	}
	return iDEX;
}

_int CPlayerInfo::Get_TotalINT()
{
	_int iINT = m_pPlayer->Get_Stat().iINT;
	for (auto& pEquipSlot : m_pEquipSlots)
	{
		const EquipItemData* pData =  static_cast<const EquipItemData*>(pEquipSlot->Get_ItemData());
		if(pData)
			iINT += pData->iINT;
	}
	return iINT;
}

_int CPlayerInfo::Get_TotalLUK()
{
	_int iLUK = m_pPlayer->Get_Stat().iLUK;
	for (auto& pEquipSlot : m_pEquipSlots)
	{
		const EquipItemData* pData = static_cast<const EquipItemData*>(pEquipSlot->Get_ItemData());
		if (pData)
			iLUK += pData->iLUK;
	}
	return iLUK;
}

_int CPlayerInfo::Get_TotalAttack()
{
	_int iATK = Get_TotalINT() * 2 + Get_TotalLUK();
	for (auto& pEquipSlot : m_pEquipSlots)
	{
		const EquipItemData* pData = static_cast<const EquipItemData*>(pEquipSlot->Get_ItemData());
		if (pData)
			iATK += pData->iATK;
	}
	return iATK;
}

_int CPlayerInfo::Get_AttackPoint()
{
	return Get_TotalAttack() * (Get_TotalCRIT() +1);
}

_int CPlayerInfo::Get_TotalDefense()
{
	Stat tStat = m_pPlayer->Get_Stat();
	_int iDefense = tStat.iSTR * 2;
	for (auto& pEquipSlot : m_pEquipSlots)
	{
		const EquipItemData* pData = static_cast<const EquipItemData*>(pEquipSlot->Get_ItemData());
		if(pData)
			iDefense += pData->iDEF + pData->iSTR * 2;
	}
	return iDefense;
} 

_int CPlayerInfo::Get_TotalHP()
{
	_int iHP = Get_TotalSTR() * 10 + m_pPlayer->Get_DefaultStat_Ref()->iHP;
	for (auto& pEquipSlot : m_pEquipSlots)
	{
		const EquipItemData* pData = static_cast<const EquipItemData*>(pEquipSlot->Get_ItemData());
		if (pData)
			iHP += pData->iHP;
	}
	return iHP;
}

_float CPlayerInfo::Get_TotalCRIT()
{
	_float fCRIT = m_pPlayer->Get_Stat().fCrit ;
	for (auto& pEquipSlot : m_pEquipSlots)
	{
		const EquipItemData* pData = static_cast<const EquipItemData*>(pEquipSlot->Get_ItemData());
		if (pData)
			fCRIT += pData->fCRIT ;
	}
	return fCRIT;
}

_int CPlayerInfo::Get_DefensePoint()
{
	return  Get_TotalDefense() + Get_TotalHP();
}

_int CPlayerInfo::Get_BattlePoint()
{
	return Get_TotalAttack() + Get_DefensePoint();
}

void CPlayerInfo::Increase_Stat(_uint iStatType)
{
	if (m_iLeftStatPoint <= 0)
		return;
	switch (iStatType)
	{
	case 0:
		m_pPlayer->Get_Stat_Ref()->iSTR++;
		break;
	case 1:
		m_pPlayer->Get_Stat_Ref()->iDEX++;
		break;
	case 2:
		m_pPlayer->Get_Stat_Ref()->iINT++;
		break;
	case 3:
		m_pPlayer->Get_Stat_Ref()->iLUK++;
		break;
	case 4:
		m_pPlayer->Get_DefaultStat_Ref()->iHP += 50;
		break;
	case 5:
		m_pPlayer->Get_Stat_Ref()->fCrit += 0.05f;
		break;
	default:
		break;
	}
	m_iLeftStatPoint--;
}




void CPlayerInfo::Free()
{
	__super::Free();
	Safe_Release(m_pPlayer);
	for (_uint i = 0; i < (_uint)EQUIP_ITEM_TYPE::LAST; i++)
		Safe_Release(m_pEquipSlots[i]);
	for (_uint i = 0; i < (_uint)DECO_ITEM_TYPE::LAST; i++)
		Safe_Release(m_pDecoSlots[i]);
}

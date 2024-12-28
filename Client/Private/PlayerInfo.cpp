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
	return m_pPlayer->Gain_Exp(iAmount);
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



void CPlayerInfo::Free()
{
	__super::Free();
	Safe_Release(m_pPlayer);
	for (_uint i = 0; i < (_uint)EQUIP_ITEM_TYPE::LAST; i++)
		Safe_Release(m_pEquipSlots[i]);
	for (_uint i = 0; i < (_uint)DECO_ITEM_TYPE::LAST; i++)
		Safe_Release(m_pDecoSlots[i]);
}

#include "stdafx.h"
#include "InvenEquipSlot.h"
#include "PlayerInfo.h"
#include "Player.h"
#include "Inventory.h"
#include "PlayerEquipSlot.h"
#include "GameInstance.h"

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
	EQUIP_ITEM_DATA* pEQDesc = static_cast<EQUIP_ITEM_DATA	*>( pInvenSlot->Get_ItemDesc());
	EQUIP_ITEM_TYPE  eEQType = pEQDesc->eEquipType;
	assert(pEQDesc != nullptr);

	//해당 부위에 장착된 아이템이 장착 해제. 
	//한벌은 SUIT, TOP, BOTTOM 모두 장착 해제해야함.
	list <EQUIP_ITEM_DATA*> pPopedItem;
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
	return E_NOTIMPL;
}

void CPlayerInfo::Toggle_UI()
{
}


void CPlayerInfo::Free()
{
	__super::Free();
	Safe_Release(m_pPlayer);
	for (_uint i = 0; i < (_uint)EQUIP_ITEM_TYPE::LAST; i++)
		Safe_Release(m_pEquipSlots[i]);
}

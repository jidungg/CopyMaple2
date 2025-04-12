#include "stdafx.h"
#include "Inventory.h"
#include "UIManager.h"
#include "GameInstance.h"
#include "UIBundle.h"
#include "UIInventory.h"
#include "UIInvenSlotEntry.h"
#include "InvenEquipSlot.h"
#include "InvenDecoSlot.h"
#include "InvenConsumableSlot.h"
#include "InvenBuildSlot.h"
#include "InvenETCSlot.h"

IMPLEMENT_SINGLETON(CInventory)

CInventory::CInventory()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pGameInstance(CGameInstance::GetInstance())
	//, m_pUI(nullptr)
{
}

HRESULT CInventory::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	for (_uint i = 0; i < (_uint)ITEM_TYPE::LAST; ++i)
	{
		m_vecSlot[i].resize(m_iMaxSlot[i]);
		for (_uint j = 0; j < m_iMaxSlot[i]; j++)
		{
			switch ((ITEM_TYPE)i)
			{
			case ITEM_TYPE::EQUIP:
				m_vecSlot[i][j] = CInvenEquipSlot::Create(j, this);
				break;
			case ITEM_TYPE::DECO:
				m_vecSlot[i][j] = CInvenDecoSlot::Create(j, this);
				break;
			case ITEM_TYPE::CONSUMABLE:
				m_vecSlot[i][j] = CInvenConsumableSlot::Create(j, this);
				break;
			case ITEM_TYPE::BUILD:
				m_vecSlot[i][j] = CInvenBuildSlot::Create(j, this);
				break;
			case ITEM_TYPE::ETC:
				m_vecSlot[i][j] = CInvenETCSlot::Create(j, this);
				break;
			case ITEM_TYPE::LAST:
			default:
				break;
			}
		}
	}
	UIBUNDLE->Get_Inventory()->Ready_Slots();
	return S_OK;
}

HRESULT CInventory::Insert_Item(const ITEM_DATA* pData, _uint iCount)
{
	assert(pData != nullptr);
	CInvenSlot* pUpdateSlot = nullptr;
	switch (pData->eITemType)
	{
	case ITEM_TYPE::EQUIP:
	case ITEM_TYPE::DECO:
		for (auto& pSlot : m_vecSlot[(_uint)pData->eITemType])
			if (S_OK == pSlot->Insert_Item(pData))
			{
				pUpdateSlot = pSlot;
				break;
			}
		break;
	default:
	case ITEM_TYPE::CONSUMABLE:
	case ITEM_TYPE::BUILD:
	case ITEM_TYPE::ETC:
	{
		CInvenSlot* pFirstEmptySlot = nullptr;
		for (auto& pSlot : m_vecSlot[(_uint)pData->eITemType])
		{
			
			if (pSlot->Is_Empty()  )
			{
				//들어갈 수 있는 첫 번째 빈 슬롯이다
				if (pFirstEmptySlot == nullptr && pSlot->Is_Insertable(pData, iCount))
					pFirstEmptySlot = pSlot;
			}
			else
			{
				//같은 아이템이 든 슬롯이다
				if (pSlot->Is_Insertable(pData, iCount))
				{
					pSlot->Insert_Item(pData, iCount);
					pUpdateSlot = pSlot;
					return S_OK;
				}
			}
		}
		//들어갈 수 있는 같은 아이템이 든 슬롯이 없었으면 첫 번째 빈 슬롯에 넣는다.
		if(S_OK==pFirstEmptySlot->Insert_Item(pData, iCount))
			pUpdateSlot = pFirstEmptySlot;
	}
	break;
	}
	if(nullptr == pUpdateSlot)
	{
		return E_FAIL;
	}
	else
	{
		//m_pUI->Update_Slot(pUpdateSlot->Get_Type(), pUpdateSlot->Get_Index());
		return S_OK;
	}

}

_bool CInventory::Is_Insertable(const ITEM_DATA* pData, _uint iCount)
{
	for (auto& pSlot : m_vecSlot[(_uint)pData->eITemType])
	{
		if (pSlot->Is_Insertable(pData))
		{
			return true;
		}
	}
	return false;
}

vector<CInvenSlot*>* CInventory::Get_Slots(ITEM_TYPE eItemType)
{
	return &m_vecSlot[(_uint)eItemType];
}

_uint CInventory::Get_ItemCount(ITEM_TYPE eItemType, _uint iItemId)
{
	_uint iCount = 0;
	for (auto& pSlot : m_vecSlot[(_uint)eItemType])
	{
		const ITEM_DATA* pData = pSlot->Get_ItemData();
		if (pData && pData->iItemID == iItemId)
			iCount += pSlot->Get_StackCount();
	}

	return iCount;
}

//void CInventory::Update_SlotUI(CInvenSlot* pSlot)
//{
//	m_pUI->Update_Slot(pSlot->Get_Type(), pSlot->Get_Index());
//}

void CInventory::Free()
{
	__super::Free();
	for (_uint i = 0; i < (_uint)ITEM_TYPE::LAST; ++i)
	{
		for (auto& pSlot : m_vecSlot[i])
			Safe_Release(pSlot);
		m_vecSlot[i].clear();
	}
}

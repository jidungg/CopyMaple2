#include "stdafx.h"
#include "DropTable.h"
#include "GameInstance.h"
#include "Client_Utility.h"
#include "WorldItem.h"
#include "ItemDataBase.h"
#include "Monster.h"

IMPLEMENT_SINGLETON(CDropTable)

CDropTable::CDropTable()
{
	m_pGameInstance = CGameInstance::GetInstance();
	m_pItemDataBase = ITEMDB;
}

HRESULT CDropTable::Load_Data()
{
	json j;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/DropTable.json", &j)))
		return E_FAIL;
	for (auto& jMonsterDrop : j["DropTable"])
	{
		MONSTER_ID eMonID = jMonsterDrop["MonsterId"];
		vector<pair<ITEM_ID_PAIR, _uint>>* pVec = new vector<pair<ITEM_ID_PAIR, _uint>>;
		for (auto& jDropItem : jMonsterDrop["Items"])
		{
			ITEM_ID_PAIR tItemID(jDropItem["Item"]);
			_uint iWeight = jDropItem["Weight"];
			pVec->push_back({ tItemID, iWeight });
		}
		m_mapDropTable.insert({ eMonID, pVec });
	}
	return S_OK;
}

void CDropTable::Drop_Item(CMonster* pMonster)
{
	MONSTER_ID eMonID = pMonster->Get_MonsterID();
	ITEM_ID_PAIR tID = Get_Drop_Item(eMonID);
	if (tID.eType == ITEM_TYPE::LAST || tID.iId == UINT_MAX)
		return;
	
	CWorldItem::WORLDITEM_DESC tItemDesc;
	tItemDesc.pItemData = m_pItemDataBase->Get_Data(tID.eType, tID.iId);
	tItemDesc.iStackCount = 1;
	CWorldItem* pItem = static_cast<CWorldItem*>(m_pGameInstance->Clone_Proto_Object_Stock(CWorldItem::m_szProtoTag, &tItemDesc));
	pItem->Set_Transform(pMonster->Get_WorldPosition());
	pItem->Compute_Matrix();
	m_pGameInstance->Add_GameObject_ToLayer((_uint)Get_CurrentTrueLevel(), (_uint)LAYERID::LAYER_WORLD_ITEM, pItem);
	pItem->PopUp();
}


ITEM_ID_PAIR CDropTable::Get_Drop_Item(MONSTER_ID _eMonID)
{
	assert(m_mapDropTable.find(_eMonID) != m_mapDropTable.end());

	auto pData = m_mapDropTable[_eMonID];
	int iTotalWeight = Get_Total_Weight(_eMonID);
	int iRand = rand() % iTotalWeight;
	int iCurWeight = 0;
	for (auto& i : *pData)
	{
		if (iRand < iCurWeight + (_int)i.second)
		{
			return i.first;
		}
		iCurWeight += i.second;
	}

	return ITEM_ID_PAIR{};
}
int CDropTable::Get_Total_Weight(MONSTER_ID eMonsterId)
{
	int result = 0;
	vector<pair<ITEM_ID_PAIR, _uint>>* pList = m_mapDropTable[eMonsterId];
	for (auto& i : *pList)
	{
		result += i.second;
	}
	return result;
}



void CDropTable::Free()
{
	__super::Free();
	for (auto& i : m_mapDropTable)
	{
		delete i.second;
	}
	m_mapDropTable.clear();
}

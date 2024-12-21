#pragma once
#include "Base.h"

#include "ObjectPool.h"
BEGIN(Engine)
class CGameInstance;
END
BEGIN(Client)
class CWorldItem;
class CItemDataBase;
class CMonster;
typedef struct ItemIdPair
{
	ItemIdPair() = default;
	ItemIdPair(json j)
	{
		eType = j["Type"];
		switch (eType)
		{
		case Client::ITEM_TYPE::EQUIP:
		{
			EQUIP_ITEM_ID eId = j["Id"];
			iId = (_uint)eId;
			break;
		}
		case Client::ITEM_TYPE::DECO:
		{
			DECO_ITEM_ID eId = j["Id"];
			iId = (_uint)eId;
			break;
		}
		case Client::ITEM_TYPE::CONSUMABLE:
		{
			CONSUMABLE_ITEM_ID eId = j["Id"];
			iId = (_uint)eId;
			break;
		}
		case Client::ITEM_TYPE::BUILD:
		{
		}
		case Client::ITEM_TYPE::ETC:
		{
			ETC_ITEM_ID eId = j["Id"];
			iId = (_uint)eId;
			break;
		}
		case Client::ITEM_TYPE::LAST:
			break;
		default:
			break;
		}
	}
	ITEM_TYPE eType = { ITEM_TYPE::LAST };
	_uint iId = { UINT_MAX };
}ITEM_ID_PAIR;
class CDropTable :
    public CBase
{
	DECLARE_SINGLETON(CDropTable)


private:
	CDropTable();
	virtual ~CDropTable() = default;

public:
	HRESULT Load_Data();
	void Drop_Item(CMonster* pMonster);

private:
	ITEM_ID_PAIR Get_Drop_Item(MONSTER_ID _eMonID);
	int Get_Total_Weight(MONSTER_ID eMonsterId );

private:
	map < MONSTER_ID, vector<pair<ITEM_ID_PAIR, _uint>>*> m_mapDropTable;

	CGameInstance* m_pGameInstance = { nullptr };
	CItemDataBase* m_pItemDataBase = { nullptr };
public:
	void Free();
};
#define DROPTABLE CDropTable::GetInstance()
END
#include "stdafx.h"
#include "MonsterDataBase.h"

IMPLEMENT_SINGLETON(CMonsterDataBase)
CMonsterDataBase::CMonsterDataBase()
{
}

HRESULT CMonsterDataBase::Load_Data()
{
	json j;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/MonsterData.json", &j)))
		return E_FAIL;
	for (auto& data : j["monsters"])
	{
		Insert_Data(new MONSTER_DATA(data));
	}
	return S_OK;
}

void CMonsterDataBase::Insert_Data(MONSTER_DATA* pData)
{
	m_mapData[pData->eMonID] = pData;
}

void CMonsterDataBase::Free()
{
	for (auto& item : m_mapData)
	{
		Safe_Delete(item.second);
	}
	m_mapData.clear();
}

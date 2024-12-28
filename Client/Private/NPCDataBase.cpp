#include "stdafx.h"
#include "NPCDataBase.h"

IMPLEMENT_SINGLETON(CNPCDataBase)
CNPCDataBase::CNPCDataBase()
{
}

HRESULT CNPCDataBase::Load_Data()
{
	json j;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/NPCData.json", &j)))
		return E_FAIL;
	for (auto& data : j["NPC"])
	{
		Insert_Data(new NPC_DATA(data));
	}
	return S_OK;
}

void CNPCDataBase::Insert_Data(NPC_DATA* pData)
{
	m_mapData[pData->eNPCId] = pData;
}

void CNPCDataBase::Free()
{
	for (auto& item : m_mapData)
	{
		Safe_Delete(item.second);
	}
	m_mapData.clear();
}

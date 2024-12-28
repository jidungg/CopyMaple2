#pragma once

#include "NPC.h"
#include "Client_Defines.h"

BEGIN(Client)
class CNPCDataBase :
    public CBase
{
	DECLARE_SINGLETON(CNPCDataBase)
private:
	CNPCDataBase();
	virtual ~CNPCDataBase() = default;

public:
	HRESULT Load_Data();
	void Insert_Data(NPC_DATA* pData);
	NPC_DATA* Get_Data(NPC_ID eID) { return m_mapData[eID]; }
private:
	map<NPC_ID, NPC_DATA*> m_mapData;
public:
	void Free();
};
#define NPCDB CNPCDataBase::GetInstance()
END 
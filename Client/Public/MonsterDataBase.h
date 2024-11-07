#pragma once

#include "Monster.h"
#include "Client_Defines.h"

BEGIN(Client)
class CMonsterDataBase :
	public CBase
{
	DECLARE_SINGLETON(CMonsterDataBase)
private:
	CMonsterDataBase();
	virtual ~CMonsterDataBase() = default;

public:
	HRESULT Load_Data();
	void Insert_Data(MONSTER_DATA* pData);
	MONSTER_DATA* Get_Data(MONSTER_ID eID) { return m_mapData[eID]; }
private:
	map<MONSTER_ID, MONSTER_DATA*> m_mapData;
public:
	void Free();
};
#define MONSTERDB CMonsterDataBase::GetInstance()
END
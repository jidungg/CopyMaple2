#pragma once
#include "Spawner.h"
BEGIN(Client)
class CMonsterSpawner :
    public CSpawner
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_MonSpawner";
private:
	explicit CMonsterSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonsterSpawner(const CMonsterSpawner& Prototype);
	virtual ~CMonsterSpawner() = default;

protected:
	virtual HRESULT Ready_SpawnObject(CSpawner::SPAWNER_DESC* pDesc) override;

public:
	static CTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);

};

END
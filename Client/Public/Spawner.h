#pragma once
#include "TerrainObject.h"
BEGIN(Client)
//몬스터 객체 하나를 가지고있음.
//
class CSpawner abstract :
    public CTerrainObject
{
public:
	typedef struct SpawnerDesc : public CTerrainObject::TERRAINOBJ_DESC
	{

	}SPAWNER_DESC;
		static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_MonSpawner";
protected:
	explicit CSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CSpawner(const CSpawner& Prototype);
	virtual ~CSpawner() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual json ToJson() override;

protected:
	virtual HRESULT Ready_SpawnObject(SPAWNER_DESC* pDesc) abstract;
protected:
	CGameObject* m_pSpawnObject = { nullptr };
	_uint m_iObjectId = { 0 };
	_float fSpawnTime = { 0.f };
	_float fTimeAcc = { 0.f };
public:
	virtual void Free() override;
};

END
#pragma once
#include "Spawner.h"
class CNPCSpanwer :
    public CSpawner
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CNPCSpanwer";
private:
	explicit CNPCSpanwer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNPCSpanwer(const CNPCSpanwer& Prototype);
	virtual ~CNPCSpanwer() = default;

protected:
	virtual HRESULT Ready_SpawnObject(CSpawner::SPAWNER_DESC* pDesc) override;

public:
	static CTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);

};


#pragma once
#include "TerrainObject.h"
BEGIN(Client)
class CPortalTerrainObject :
    public CTerrainObject
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_PortalTerrainObj";

protected:
	explicit CPortalTerrainObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CPortalTerrainObject(const CPortalTerrainObject& Prototype);
	virtual ~CPortalTerrainObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual json ToJson();
	LEVELID Get_LinkedLevelID() { return m_eLevelID; }
private:
	LEVELID m_eLevelID = { LEVELID::LAST };

public:
	static CPortalTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;

};

END

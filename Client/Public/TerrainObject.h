#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Clinet)
class CTerrainObject :
    public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		string modleName;
		_float3 Pos = { 0,0,0 };
		_float3 Rot = { 0,0,0 };
		size_t iteration = 1;
		void* pArg = nullptr;

	}TERRAINOBJ_DESC;
	
protected:
	explicit CTerrainObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CTerrainObject(const CTerrainObject& Prototype);
	virtual ~CTerrainObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
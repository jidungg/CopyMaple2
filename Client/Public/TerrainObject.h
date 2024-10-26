#pragma once
#include "ModelObject.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CTerrainObject :
	public CModelObject
{
public:
	typedef struct TerrainObjDesc: public CModelObject::MODELOBJ_DESC
	{
		TERRAIN_OBJ_TYPE eType = TERRAIN_OBJ_TYPE::LAST;
		_float4 pos = {0,0,0,1};
		_char  strItemName[MAX_PATH];
		int data = 0;//MonsterSpawner 
		_uint index = 0;
	}TERRAINOBJ_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_TerrainObject";
protected:
	explicit CTerrainObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CTerrainObject(const CTerrainObject& Prototype);
	virtual ~CTerrainObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;

	void Set_TerrainDir(DIRECTION eDir) { m_eTerrainDir = eDir; }
	_uint Get_Index() { return m_iIndex; }
	string Get_ModelName() { return m_strItemName; }
	DIRECTION Get_Direction() { return m_eTerrainDir; }

	virtual json ToJson();
	void Rotate();
private:
	HRESULT Ready_Components(TERRAINOBJ_DESC* pDesc);

protected:
	TERRAIN_OBJ_TYPE m_eTerrObjType = TERRAIN_OBJ_TYPE::LAST;
	string m_strItemName;
	_uint m_iIndex = 0;
	DIRECTION m_eTerrainDir = DIRECTION::LAST;
	CCollider* m_pColliderCom = {nullptr};

public:
	static CTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once
#include "ModelObject.h"

BEGIN(Engine)
class CColliderBase;
END

BEGIN(Client)
class CCharacter;
class CCubeTerrain;
class CTerrainObject :
	public CModelObject
{
public:
	typedef struct TerrainObjDesc: public CModelObject::MODELOBJ_DESC
	{
		BUILD_ITEM_ID eID = BUILD_ITEM_ID::LAST;
		_float4 pos = {0,0,0,1};
		_uint index = 0;
		vector<_int> vecIData ;
		vector<_float> vecFData ;
		CCubeTerrain* pCubeTerrain = { nullptr };
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
	

	virtual json ToJson();
	void Rotate();

	_uint Get_Index() { return m_iIndex; }
	DIRECTION Get_Direction() { return m_eTerrainDir; }
	BUILD_ITEM_ID Get_BuildItemID() { return m_eBuildItemID; }
	_vector BolckXZ(_vector vPosition, _vector vDirection, _float fMoveDistance, _float fCollisionRadius, _float fCollisionHeight);
	_float Get_TopHeight(_vector Pos);
	_float Get_BottomHeight(_vector Pos);
	_float Get_BuildItemType() { return (_float)m_eBuildItemType; }
	_bool Is_BlockingType() { return m_eBuildItemType == BUILD_ITEM_TYPE::GROUND || m_eBuildItemType == BUILD_ITEM_TYPE::FILED_BLOCK; }
	_bool RayCast(const Ray& tRay, RaycastHit* pOut);

	void Set_TerrainDir(DIRECTION eDir) { m_eTerrainDir = eDir; }
private:
	HRESULT Ready_Components(TERRAINOBJ_DESC* pDesc);

protected:
	BUILD_ITEM_ID m_eBuildItemID = BUILD_ITEM_ID::LAST;
	BUILD_ITEM_TYPE m_eBuildItemType = BUILD_ITEM_TYPE::LAST;
	_uint m_iIndex = 0;
	DIRECTION m_eTerrainDir = DIRECTION::LAST;
	CColliderBase* m_pCubeColliderCom = {nullptr};
	CColliderBase* m_pMeshColliderCom = {nullptr};
	_bool m_bRotating = false;
public:
	static CTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
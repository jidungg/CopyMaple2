#pragma once
#include "ModelObject.h"

BEGIN(Engine)
class CColliderBase;
class CCollider_AABB;
class CCollider_Mesh;
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
		_uint iID = { UINT_MAX };
		_float4 pos = {0,0,0,1};
		_uint index = { UINT_MAX };
		_uint iParentIndex = { UINT_MAX };
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
	_uint Get_BuildItemID() { return m_iBuildItemID; }
	_vector BolckXZ(_vector vPosition, _vector vDirection, _float fMoveDistance, _float fCollisionRadius, _float fCollisionHeight);
	_float Get_TopHeight(_vector Pos);
	_float Get_BottomHeight(_vector Pos);
	BUILD_ITEM_TYPE Get_BuildItemType() { return m_eBuildItemType; }
	_bool Is_BlockingType() { return m_eBlockType != BUILD_ITEM_BLOCK_TYPE::NON_BLOCK; }
	_bool RayCast(const Ray& tRay, RaycastHit* pOut);
	void Culling(_float fRange = 0);

	void Set_TerrainDir(DIRECTION eDir) { m_eTerrainDir = eDir; }
private:
	HRESULT Ready_Components(TERRAINOBJ_DESC* pDesc);

protected:
	_uint m_iBuildItemID = { UINT_MAX };
	BUILD_ITEM_BLOCK_TYPE m_eBlockType = { BUILD_ITEM_BLOCK_TYPE::LAST };
	BUILD_ITEM_TYPE m_eBuildItemType = { BUILD_ITEM_TYPE::LAST };
	_uint m_iIndex = { UINT_MAX };
	_uint m_iParentIndex = { UINT_MAX };
	DIRECTION m_eTerrainDir = DIRECTION::LAST;
	CCollider_AABB* m_pCubeColliderCom = {nullptr};
	CCollider_Mesh* m_pMeshColliderCom = {nullptr};
	_bool m_bRotating = false;
public:
	static CTerrainObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once

#include "UIObject.h"
#include "TerrainObject.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CModel;
END


BEGIN(Client)
#define CELL_SIZE 1
#define STRAIGHT_DIST CELL_SIZE
#define DIAG_DIST 1.414f
#define TRIAG_DIST 1.732f

class CTerrainObject;
class CCubeTerrain final : public CGameObject
{
	enum class CELL_RELATION
	{
		STRAIGHT,
		DIAG,
		TRIAG,
		SAME,
		LAST
	};
private:
	CCubeTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* szMapFileName);
	CCubeTerrain(const CCubeTerrain& Prototype);
	virtual ~CCubeTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	//virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	HRESULT Add_TerrainObject( CTerrainObject::TERRAINOBJ_DESC& tDesc);
	HRESULT Remove_TerrainObject(_uint Index);
	_vector IndexToPos(_uint Index);
	_uint PosToIndex(const _float4& Pos);
	_int PosToIndex(const _fvector& Pos);
	XMUINT3 SplitIndex(_uint iIdx);
	_uint CombineIndex(XMUINT3 i3Idx);

	_bool Is_InSide(_vector Pos);
	_bool Is_ValidIndex(XMUINT3 i3Index);
	XMUINT3 Get_Size() { return m_vSize; }
	CTerrainObject* Get_TerrainObject(_uint Index);
	_float Get_FloorHeight(_vector Pos);
	_float Get_CelingHeight(_vector Pos);
	void Get_AdjCells(_uint Index, vector<_uint>& vecAdjCells);
	void Get_XZAdjCells(_uint Index, vector<_uint>& vecAdjCells);
	_float Get_Distance(_uint StartIndex, _uint DestIndex);
	_float Get_AdjDistance(_uint StartIndex, _uint DestIndex);
	CELL_RELATION Get_AdjCell_Relation(_uint StartIndex, _uint DestIndex);
	CTerrainObject* Get_Portal(LEVELID eLinkedLevel);
	CTerrainObject* Get_PlayerSpawn();
	_vector Get_ContainedCellPosition(const _fvector& Pos);


	HRESULT Save_To_Json(string strNewFilepath);
	HRESULT Load_From_Json(string strJsonFilePath);
	_vector BlockXZ(CCharacter* pCharacter);
	_bool RayCastXZ(const Ray& tRay, RaycastHit* pOut);
	_bool RayCast(const Ray& tRay, RaycastHit* pOut);

private:
	string m_strJsonFilePath;
	XMUINT3 m_vSize = { 1, 1, 1 };
	
	vector<CTerrainObject*> m_vecCells;
public:
	static CCubeTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* szMapFileName);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END
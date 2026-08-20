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

constexpr _float CELL_SIZE = 1.f;
constexpr _float STRAIGHT_DIST = CELL_SIZE;
constexpr _float DIAG_DIST = 1.414f;
constexpr _float TRIAG_DIST = 1.732f;
class COctoTree;
class CTerrainObject;
class TerrainObjectFactory
{
public:
	static CTerrainObject* Create(
		BUILD_ITEM_TYPE type,
		CGameInstance* pGI,
		CTerrainObject::TERRAINOBJ_DESC& desc);
};
class CTerrain final : public CGameObject
{
public:
	enum class CULLING_MODE
	{
		NONE,		// 컬링 없이 전체 렌더
		PER_CELL,	// 셀 단위 프러스텀 검사
		OCTREE,		// 옥트리 순회 후 셀 단위 프러스텀 검사
	};
private:
	enum class CELL_RELATION
	{
		STRAIGHT,
		DIAG,
		TRIAG,
		SAME,
		LAST
	};
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* szMapFileName);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

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
	_vector SnapPosition(_vector Pos);
	XMUINT3 SplitIndex(_uint iIdx);
	_uint CombineIndex(XMUINT3 i3Idx);
	HRESULT Save_To_Json(string strNewFilepath);
	HRESULT Load_From_Json(string strJsonFilePath);
	_vector Blocking(CCharacter* pCharacter);
	_vector Blocking(CCharacter* pCharacter, _uint iCheckRange);
	_bool RayCast(const Ray& tRay, RaycastHit* pOut);
	void Culling(COctoTree* pOctoTree);
	void Set_CullingMode(CULLING_MODE eMode) { m_eCullingMode = eMode; }
	CULLING_MODE Get_CullingMode() const { return m_eCullingMode; }

	_bool Is_Buildable(_vector Pos);
	_bool Is_InSide(_vector Pos);
	_bool Is_ValidIndex(_uint iIndex);
	_bool Is_ValidIndex(XMUINT3 i3Index);
	XMUINT3 Get_Size() { return m_vSize; }
	CTerrainObject* Get_TerrainObject(_uint Index);
	_float Get_FloorHeight(_vector Pos, _uint iCheckRange = 0);
	_float Get_CelingHeight(_vector Pos, _uint iCheckRange = 0);
	void Get_AdjWayFinderCells(_uint Index, vector<_uint>& vecAdjCells);
	void Get_XZAdjWayFinderCells(_uint Index, vector<_uint>& vecAdjCells);
	_float Get_Distance(_uint StartIndex, _uint DestIndex);
	_float Get_AdjDistance(_uint StartIndex, _uint DestIndex);
	CELL_RELATION Get_AdjCell_Relation(_uint StartIndex, _uint DestIndex);
	CTerrainObject* Get_Portal(LEVELID eLinkedLevel);
	CTerrainObject* Get_PlayerSpawn();
	_vector Get_ContainedCellPosition(const _fvector& Pos);
	void Get_ContainingCells(CColliderBase* pCollider, list<_uint>& vecCells);

private:
	void Cull_Cell(CTerrainObject* pCell);
	void Benchmark_ColliderRayCast();	// NUM8: Mesh vs Box 콜라이더 RayCast 마이크로벤치

private:
	string m_strJsonFilePath;
	XMUINT3 m_vSize = { 1, 1, 1 };
	
	vector<CTerrainObject*> m_vecCubes;
	vector<CTerrainObject*> m_vecNonCubes;
	COctoTree* m_pOctoTree = { nullptr };
	_uint iTmpCellCount = 0;
	_float fCullDistance = 1.7f;
	CULLING_MODE m_eCullingMode = { CULLING_MODE::PER_CELL };

	// 컬링 성능 계측용 (60프레임 단위 평균 출력)
	_uint	m_iDrawCallCount = 0;
	_uint	m_iAccumDrawCalls = 0;
	_double	m_dAccumCullingTimeMs = 0.0;
	_uint	m_iAccumFrames = 0;
	_bool	m_bCullBench = false;	// NUM9로 토글: 컬링 모드 자동 순환 벤치
public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* szMapFileName);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END
#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"
#include "JsonParser.h"
#include "ItemDataBase.h"
#include "Character.h"
#include "MonsterSpawner.h"
#include "NPCSpanwer.h"
#include "PortalTerrainObject.h"
#include "Collider.h"
#include "OctoTree.h"


CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const char* szMapFileName)
	: CGameObject { pDevice, pContext }
{
	m_strJsonFilePath = szMapFileName;
}

CTerrain::CTerrain(const CTerrain & Prototype)
	: CGameObject { Prototype },
	m_strJsonFilePath{ Prototype.m_strJsonFilePath }
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CTerrain::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Load_From_Json(m_strJsonFilePath)))
		return E_FAIL;
	_uint iLBN = 0;
	_uint iRTF = m_vSize.x * m_vSize.y * m_vSize.z -1;

	m_pOctoTree = COctoTree::Create(m_vSize, iLBN, iRTF);
	//string strOctoTree;
	//m_pOctoTree->To_String(strOctoTree, 0);
	//cout << strOctoTree << endl;
	return S_OK;
}


void CTerrain::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	// [프레임타임] 직전 Late_Update 호출과의 간격 = 전체 프레임 시간(ms)
	{
		LARGE_INTEGER tNow, tFrameFreq;
		QueryPerformanceCounter(&tNow);
		QueryPerformanceFrequency(&tFrameFreq);
		if (0 != m_tPrevFrame.QuadPart)
			m_dAccumFrameTimeMs += (_double)(tNow.QuadPart - m_tPrevFrame.QuadPart) * 1000.0 / (_double)tFrameFreq.QuadPart;
		m_tPrevFrame = tNow;
	}

	if (m_pGameInstance->GetKeyState(KEY::NUM0) == KEY_STATE::DOWN)
	{
		Ray tBodyRay({ 1,1,1 }, XMVector3Normalize({ 1,1,1 }), 1.0f);
		RaycastHit tBodyhit;
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < 100000; i++)
		{
			RayCast(tBodyRay, &tBodyhit); // 테스트할 코드
		}

		auto end = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		std::cout << duration.count() / 100000.0 << " us\n";
	}
	// [벤치] NUM8: Mesh vs Box 콜라이더 RayCast 마이크로벤치 (1회성)
	if (KEY_STATE::DOWN == m_pGameInstance->GetKeyState(KEY::NUM8))
		Benchmark_ColliderRayCast();
	// [벤치] NUM9: 컬링 모드 자동 순환(NONE→PER_CELL→OCTREE) on/off
	if (KEY_STATE::DOWN == m_pGameInstance->GetKeyState(KEY::NUM9))
	{
		switch (m_eCullingMode)
		{
		case CULLING_MODE::NONE:		m_eCullingMode = CULLING_MODE::PER_CELL;	break;
		case CULLING_MODE::PER_CELL:	m_eCullingMode = CULLING_MODE::OCTREE;		break;
		case CULLING_MODE::OCTREE:		m_eCullingMode = CULLING_MODE::NONE;			break;
		}
	}

	iTmpCellCount = 0;
	m_iDrawCallCount = 0;

	LARGE_INTEGER tFreq, tStart, tEnd;
	QueryPerformanceFrequency(&tFreq);
	QueryPerformanceCounter(&tStart);

	switch (m_eCullingMode)
	{
	case CULLING_MODE::NONE:
		for (auto& pCell : m_vecCubes)
		{
			if (nullptr == pCell)
				continue;
			m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, pCell);
			++m_iDrawCallCount;
		}
		break;

	case CULLING_MODE::PER_CELL:
		for (auto& pCell : m_vecCubes)
		{
			if (nullptr == pCell)
				continue;
			Cull_Cell(pCell);
		}
		break;

	case CULLING_MODE::OCTREE:
		Culling(m_pOctoTree);
		break;
	}

	QueryPerformanceCounter(&tEnd);
	m_dAccumCullingTimeMs += (_double)(tEnd.QuadPart - tStart.QuadPart) * 1000.0 / (_double)tFreq.QuadPart;
	m_iAccumDrawCalls += m_iDrawCallCount;
	++m_iAccumFrames;

	if (m_iAccumFrames >= 60)
	{
		const char* szMode = "None";
		if (CULLING_MODE::PER_CELL == m_eCullingMode)		szMode = "PerCell";
		else if (CULLING_MODE::OCTREE == m_eCullingMode)	szMode = "Octree";

		_double dAvgFrameMs = m_dAccumFrameTimeMs / m_iAccumFrames;
		_double dFPS = (dAvgFrameMs > 0.0) ? (1000.0 / dAvgFrameMs) : 0.0;

		cout << "[Culling] Mode=" << szMode
			<< " AvgDrawCalls=" << (m_iAccumDrawCalls / m_iAccumFrames)
			<< " AvgCullingTime=" << (m_dAccumCullingTimeMs / m_iAccumFrames) << "ms"
			<< " AvgFrameTime=" << dAvgFrameMs << "ms"
			<< " FPS=" << dFPS
			<< " TotalCells=" << m_vecCubes.size() << endl;

		m_dAccumCullingTimeMs = 0.0;
		m_dAccumFrameTimeMs = 0.0;
		m_iAccumDrawCalls = 0;
		m_iAccumFrames = 0;

	}


}




HRESULT CTerrain::Load_From_Json(string strJsonFilePath)
{
	CItemDataBase* pDB = ITEMDB;
	json j;
	if (FAILED(CJsonParser::ReadJsonFile(strJsonFilePath.c_str(), &j)))
		return E_FAIL;

	m_vSize = { j["size"][0],j["size"][1],j["size"][2] };
	m_vecCubes.resize(m_vSize.x * m_vSize.y * m_vSize.z, nullptr);
	for (const auto& item : j["cells"]) {
		_int eId =  item["ItemId"];
		size_t iteration = item["Iteration"];
		_uint terrIdx = item["Index"];
		BUILD_ITEM_DATA* pBuildItemDesc = static_cast<BUILD_ITEM_DATA*>( pDB->Get_Data(ITEM_TYPE::BUILD,(_uint)eId));

		CTerrainObject::TERRAINOBJ_DESC desc;
		desc.fRotationPerSec = 5.f;
		desc.fSpeedPerSec = 1.f;
		lstrcpyW(desc.szModelProtoName, pBuildItemDesc->strModelTag);
		desc.eModelProtoLevelID = LEVEL_LOADING;
		desc.iID = eId;
		desc.direction = item["Direction"];
		desc.vecIData = item["IntData"].get<vector<_int>>();
		desc.vecFData = item["FloatData"].get<vector<_float>>();

		for (int i = 0 ; i < iteration; i++)
		{
			desc.index = terrIdx + i;
			desc.iParentIndex = desc.index;
			XMStoreFloat4(&desc.pos, IndexToPos(desc.index));
 			if(FAILED(Add_TerrainObject(desc)))
				return E_FAIL;
		}
	}
	return S_OK;
}

_vector CTerrain::Blocking(CCharacter* pCharacter)
{
	return Blocking(pCharacter, pCharacter->Get_BlockingRange());
}

_vector CTerrain::Blocking(CCharacter* pCharacter, _uint iCheckRange)
{
	iCheckRange = 10000;

	_vector vPos = pCharacter->Get_TransformPosition();
	_vector vMoveDir = pCharacter->Get_MoveDirection();
	_float fBodyCollisionRadius = pCharacter->Get_BodyCollisionRadius();
	_float3 fBodyCollisionOffset = pCharacter->Get_BodyCollisionOffset();
	_float fMoveDistance = pCharacter->Get_MoveDistance();
	_float fXForce = XMVectorGetX(vMoveDir);
	_float fZForce = XMVectorGetZ(vMoveDir);
	_float fYForce = XMVectorGetY(vMoveDir);

	//검사할 셀의 인덱스 범위 계산
	_int iIdx = PosToIndex(vPos);
	if (iIdx < 0)
		return  vPos + vMoveDir * fMoveDistance;
	_int iYIdx = iIdx / (m_vSize.x * m_vSize.z);
	_int iXIdx = iIdx % m_vSize.x;
	_int iZIdx = iIdx / m_vSize.x % m_vSize.z;

	_int iMinX = iXIdx, iMaxX = iXIdx, iMinZ = iZIdx, iMaxZ = iZIdx, iMinY = iYIdx, iMaxY = iYIdx;

	iMaxX += iCheckRange;
	iMinX -= iCheckRange;
	iMaxZ += iCheckRange;
	iMinZ -= iCheckRange;
	iMaxY += iCheckRange;

	iMaxX = min(iMaxX, m_vSize.x - 1);
	iMinX = max(iMinX, 0);
	iMaxZ = min(iMaxZ, m_vSize.z - 1);
	iMinZ = max(iMinZ, 0);
	iMaxY = min(iMaxY, m_vSize.y - 1);
	iMinY = max(iMinY, 0);
	_vector vNextPos = vPos;
	//검사 범위 내의 셀들에 대해서만 선택적으로 충돌 검사
	for (_uint iY = iMinY; iY < iMaxY; iY++)
	{
		for (_uint iZ = iMinZ; iZ <= iMaxZ; iZ++)
		{
			for (_uint iX = iMinX; iX <= iMaxX; iX++)
			{
				_uint iTmpIdx = CombineIndex({ iX,iY,iZ });
				

				if (nullptr == m_vecCubes[iTmpIdx])
					continue;
				if (false == m_vecCubes[iTmpIdx]->Is_BlockingType())
					continue;
				if (iTmpIdx == iIdx)
					continue;
				
				vNextPos = m_vecCubes[iTmpIdx]->BolckXZ(vPos, vMoveDir, fMoveDistance, fBodyCollisionRadius, fBodyCollisionOffset.y);
				
				_vector vTmp = vNextPos - vPos;
				vMoveDir = XMVector4Normalize(vTmp);
				fMoveDistance = XMVectorGetX(XMVector3Length(vTmp));

				
			}
		}
	}
	vNextPos = vPos + fMoveDistance * vMoveDir;
	return vNextPos;
}


_bool CTerrain::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	_vector vOrigin = tRay.vOrigin;
	_vector vDir = XMVector3Normalize(tRay.vDirection);
	//dir 벡터가 0인 경우 예외 처리
	if (XMVector3Equal(vDir, XMVectorZero()))
		return false;

	// X/Z 중심 보정
	_vector vPos = vOrigin + XMVectorSet(0.5f, 0.f, 0.5f, 0.f);

	// 현재 셀 좌표
	_int iX = (_int)(XMVectorGetX(vPos));
	_int iY = (_int)(XMVectorGetY(vPos));
	_int iZ = (_int)(XMVectorGetZ(vPos));

	_float fDirX = XMVectorGetX(vDir);
	_float fDirY = XMVectorGetY(vDir);
	_float fDirZ = XMVectorGetZ(vDir);

	//셀 인덱스 증가 방향
	_int iStepX = (fDirX > 0) ? 1 : (fDirX < 0 ? -1 : 0);
	_int iStepY = (fDirY > 0) ? 1 : (fDirY < 0 ? -1 : 0);
	_int fStepZ = (fDirZ > 0) ? 1 : (fDirZ < 0 ? -1 : 0);

	//셀 1칸 이동하는데 걸리는 t값
	_float fTDeltaX = (fDirX != 0) ? fabsf(1.f / fDirX) : FLT_MAX;
	_float fTDeltaY = (fDirY != 0) ? fabsf(1.f / fDirY) : FLT_MAX;
	_float fTDeltaZ = (fDirZ != 0) ? fabsf(1.f / fDirZ) : FLT_MAX;

	//
	_float fNextBoundaryX = iX + (iStepX > 0 ? 0.5f : -0.5f);
	_float fNextBoundaryY = iY + (iStepY > 0 ? 1.f  : 0.f);
	_float fNextBoundaryZ = iZ + (fStepZ > 0 ? 0.5f : -0.5f);

	//다음 경계면에서의 t값
	_float fTMaxX = (fDirX != 0) ? (fNextBoundaryX - XMVectorGetX(vPos)) / fDirX : FLT_MAX;
	_float fTMaxY = (fDirY != 0) ? (fNextBoundaryY - XMVectorGetY(vPos)) / fDirY : FLT_MAX;
	_float fTMaxZ = (fDirZ != 0) ? (fNextBoundaryZ - XMVectorGetZ(vPos)) / fDirZ : FLT_MAX;

	//현재 Ray 파라미터
	_float fT = 0.f;

	while (fT <= tRay.fDist)
	{
		_vector vRealPos = XMVectorSet(iX, iY, iZ, 0.f)
			- XMVectorSet(0.5f, 0.f, 0.5f, 0.f);

		_int iIdx = PosToIndex(vRealPos);
		if (iIdx < 0)
			return false;
		if (m_vecCubes[iIdx] && m_vecCubes[iIdx]->RayCast(tRay, pOut))
			return true;

		// DDA step
		if (fTMaxX < fTMaxY && fTMaxX < fTMaxZ)
		{
			iX += iStepX;
			fT = fTMaxX;
			fTMaxX += fTDeltaX;
		}
		else if (fTMaxZ < fTMaxY)
		{
			iZ += fStepZ;
			fT = fTMaxZ;
			fTMaxZ += fTDeltaZ;
		}
		else
		{
			iY += iStepY;
			fT = fTMaxY;
			fTMaxY += fTDeltaY;
		}
	}
	//for (auto& c : m_vecCubes)
	//{
	//	if (c)
	//		if (c->RayCast(tRay, pOut))
	//			return true;

	//}
	return false;
}

void CTerrain::Cull_Cell(CTerrainObject* pCell)
{
	if (false == m_pGameInstance->Frustum_Culling_World(pCell->Get_WorldPosition(), fCullDistance))
		return;

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, pCell);
	++m_iDrawCallCount;
}

void CTerrain::Benchmark_ColliderRayCast()
{
	// 맵에서 Box 타입(FLOOR/WALL/GROUND)과 Mesh 타입 오브젝트를 하나씩 자동 선정
	CTerrainObject* pBox = nullptr;
	CTerrainObject* pMesh = nullptr;

	for (auto& pCell : m_vecCubes)
	{
		if (nullptr == pCell || false == pCell->Is_BlockingType())
			continue;

		BUILD_ITEM_TYPE eType = pCell->Get_BuildItemType();
		_bool bBoxType = (Client::BUILD_ITEM_TYPE::FLOOR == eType
			|| Client::BUILD_ITEM_TYPE::WALL == eType
			|| Client::BUILD_ITEM_TYPE::GROUND == eType);

		if (bBoxType && nullptr == pBox)		pBox = pCell;
		else if (!bBoxType && nullptr == pMesh)	pMesh = pCell;

		if (nullptr != pBox && nullptr != pMesh)
			break;
	}

	const _int iIter = 100000;
	LARGE_INTEGER tFreq;
	QueryPerformanceFrequency(&tFreq);

	// 대상 오브젝트 위에서 아래로 쏘는 Ray로 RayCast를 iIter회 반복 → 평균 μs
	auto benchOne = [&](CTerrainObject* pTarget) -> _double
	{
		if (nullptr == pTarget)
			return -1.0;

		_vector vPos = pTarget->Get_WorldPosition();
		Ray ray(vPos + XMVectorSet(0.f, 2.f, 0.f, 0.f), XMVectorSet(0.f, -1.f, 0.f, 0.f), 5.f);
		RaycastHit hit;

		LARGE_INTEGER tStart, tEnd;
		QueryPerformanceCounter(&tStart);
		for (_int i = 0; i < iIter; ++i)
			pTarget->RayCast(ray, &hit);
		QueryPerformanceCounter(&tEnd);

		return (_double)(tEnd.QuadPart - tStart.QuadPart) * 1e6 / (_double)tFreq.QuadPart / (_double)iIter;
	};

	_double dBoxUs = benchOne(pBox);
	_double dMeshUs = benchOne(pMesh);

	cout << "[ColliderBench] N=" << iIter << " per target" << endl;
	if (nullptr != pBox)	cout << "  Box  (idx=" << pBox->Get_Index() << ") avg = " << dBoxUs << " us" << endl;
	else					cout << "  Box  : no target found" << endl;
	if (nullptr != pMesh)	cout << "  Mesh (idx=" << pMesh->Get_Index() << ") avg = " << dMeshUs << " us" << endl;
	else					cout << "  Mesh : no target found" << endl;
	if (nullptr != pBox && nullptr != pMesh && dBoxUs > 0.0)
		cout << "  Mesh/Box ratio = " << (dMeshUs / dBoxUs) << " x" << endl;
}

void CTerrain::Culling(COctoTree* pOctoTree)
{
	if (nullptr == pOctoTree)
		return;

	_uint* pIndices = pOctoTree->Get_Corners();
	_float		fRange = XMVectorGetX(XMVector3Length(IndexToPos(pOctoTree->Get_Center()) - IndexToPos(pIndices[COctoTree::RTF])));

	_vector vCenterPos = IndexToPos(pOctoTree->Get_Center());
	if (true == m_pGameInstance->Frustum_Culling_World(vCenterPos, fRange + fCullDistance))
	{
		if (pOctoTree->Is_Leaf())
		{
			_uint iSIze = pOctoTree->Get_MaxSize();
			iTmpCellCount += pOctoTree->Get_CellCount();
			if (2 == iSIze)
			{
				for (_uint i = 0; i < COctoTree::CORNER_END; i++)
					if (UINT_MAX != pIndices[i] && nullptr != m_vecCubes[pIndices[i]])
						Cull_Cell(m_vecCubes[pIndices[i]]);
			}
			else if (1 >= iSIze)
				if (UINT_MAX != pIndices[0] && nullptr != m_vecCubes[pIndices[0]])
					Cull_Cell(m_vecCubes[pIndices[0]]);
		}
		else
		{
			for (_uint i = 0; i < COctoTree::CORNER_END; i++)
			{
				COctoTree* pChild = pOctoTree->Get_Child((COctoTree::CORNER)i);
				if (nullptr != pChild)
					Culling(pChild);
			}
		}

	}
}

HRESULT CTerrain::Save_To_Json(string strNewFilepath)
{
	json j;
	j["cells"] = json::array();
	json& jCells = j["cells"];
	json jCurObj;
	bool bFirst = true;
	for (auto& cell : m_vecCubes)
	{
		if (cell == nullptr) continue;
		CTerrainObject* pTerrainObj = static_cast<CTerrainObject*>(cell);
		if (bFirst)
		{
			jCurObj = pTerrainObj->ToJson();
			bFirst = false;
			continue;
		}
		json jTemp = pTerrainObj->ToJson();
		int curIdx = jCurObj["Index"];
		int curIter = jCurObj["Iteration"];

		if ( jCurObj["ItemId"] == jTemp["ItemId"]
			&& jCurObj["IntData"] == jTemp["IntData"]
			&& jCurObj["FloatData"] == jTemp["FloatData"]
			&& jCurObj["Direction"] == jTemp["Direction"]
			&& (curIdx +curIter) == jTemp["Index"])
		{
			jCurObj["Iteration"] = jCurObj["Iteration"] + 1;
		}
		else
		{
			jCells.push_back(jCurObj);
			jCurObj = jTemp;
		}

	}
	jCells.push_back(jCurObj);
	j["size"] = { m_vSize.x,m_vSize.y,m_vSize.z };

	CJsonParser::SaveJsonFile(strNewFilepath.c_str(),j);
	return S_OK;
}

_vector CTerrain::IndexToPos(_uint Index)
{
	return XMVectorSet(Index % m_vSize.x, 
		Index / (m_vSize.x * m_vSize.z),
		Index / m_vSize.x % m_vSize.z,
		1);
}

_uint CTerrain::PosToIndex(const _float4& Pos)
{
	_uint x = static_cast<_uint>(roundf( Pos.x));
	_uint y = static_cast<_uint>(floorf(Pos.y)); // 0~1 = 0
	_uint z = static_cast<_uint>(roundf(Pos.z));

	return x + m_vSize.x * z + m_vSize.x * m_vSize.z * y;
}

_int CTerrain::PosToIndex(const _fvector& Pos)
{
	_int x = static_cast<_int>(roundf(XMVectorGetX( Pos)));
	_int y = static_cast<_int>(floorf(XMVectorGetY(Pos))); // 0~1 = 0
	_int z = static_cast<_int>(roundf(XMVectorGetZ(Pos)));
	if (x < 0 || x >= (_int)m_vSize.x)
		return -1;
	if (y < 0 || y >= (_int)m_vSize.y)
		return -1;
	if (z < 0 || z >= (_int)m_vSize.z)
		return -1;
	return x + m_vSize.x * z + m_vSize.x * m_vSize.z * y;
}

_vector CTerrain::SnapPosition(_vector Pos)
{
	Pos.m128_f32[0] = roundf(Pos.m128_f32[0]);
	Pos.m128_f32[1] = floorf(Pos.m128_f32[1]);
	Pos.m128_f32[2] = roundf(Pos.m128_f32[2]);
	return Pos;
}

XMUINT3 CTerrain::SplitIndex(_uint iIdx)
{
	return XMUINT3{ iIdx % m_vSize.x ,iIdx / (m_vSize.x * m_vSize.z),iIdx / m_vSize.x % m_vSize.z, };
}

_uint CTerrain::CombineIndex(XMUINT3 i3Idx)
{
	return i3Idx.x + m_vSize.x * i3Idx.z + m_vSize.x * m_vSize.z * i3Idx.y;
}

HRESULT CTerrain::Add_TerrainObject( CTerrainObject::TERRAINOBJ_DESC& tDesc)
{
	if (false == Is_ValidIndex(SplitIndex( tDesc.index)))
		return E_FAIL;
	if (m_vecCubes[tDesc.index] != nullptr)
		return E_FAIL;
	tDesc.pCubeTerrain = this;
	BUILD_ITEM_TYPE eType = static_cast<BUILD_ITEM_DATA*>(ITEMDB->Get_Data(ITEM_TYPE::BUILD, (_uint)tDesc.iID))->eBuildType;

	CTerrainObject* pGameObject = TerrainObjectFactory::Create(eType, m_pGameInstance, tDesc);
	if(nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	m_vecCubes[tDesc.index] = pGameObject;
	return S_OK;
}

HRESULT CTerrain::Remove_TerrainObject(_uint Index)
{
	if (Index >= (_uint)m_vecCubes.size())
		return E_FAIL;
	if (m_vecCubes[Index] == nullptr)
		return E_FAIL;
	m_vecCubes[Index]->Set_Dead();
	Safe_Release(m_vecCubes[Index]);

	return S_OK;
}

CTerrainObject* CTerrain::Get_TerrainObject(_uint Index)
{
	if (Index < (_uint)m_vecCubes.size())
		return m_vecCubes[Index];
	return nullptr;
}

_float CTerrain::Get_FloorHeight(_vector Pos, _uint iCheckRange)
{
	_int index = PosToIndex(Pos);
	if (index < 0)
		return -1;
	_uint iYIdx = index / (m_vSize.x * m_vSize.z);
	_uint iXIdx = index % m_vSize.x;
	_uint iZIdx = index / m_vSize.x % m_vSize.z;
	
	_int iMinXIdx = iXIdx - iCheckRange;
	_int iMaxXIdx = iXIdx + iCheckRange;
	_int iMinZIdx = iZIdx - iCheckRange;
	_int iMaxZIdx = iZIdx + iCheckRange;
	iMinXIdx = max(iMinXIdx, 0);
	iMaxXIdx = min(iMaxXIdx, (_int)m_vSize.x - 1);
	iMinZIdx = max(iMinZIdx, 0);
	iMaxZIdx = min(iMaxZIdx, (_int)m_vSize.z - 1);

	for (_int yIdx = (_int)iYIdx; yIdx >= 0; yIdx--)
	{
		for (_int zIdx = iMinZIdx; zIdx <= iMaxZIdx; zIdx++)
		{
			for (_int xIdx = iMinXIdx; xIdx <= iMaxXIdx; xIdx++)
			{
				_uint iTmpIndex = xIdx + m_vSize.x * zIdx + m_vSize.x * m_vSize.z * yIdx;
				if (m_vecCubes[iTmpIndex] != nullptr)
				{
					if (false == m_vecCubes[iTmpIndex]->Is_BlockingType()) continue;

					_float fHeight = m_vecCubes[iTmpIndex]->Get_TopHeight(Pos);
					if (fHeight < 0) continue;
					return fHeight;
				}
			}
		}


		
	}
	return -1;
}

_float CTerrain::Get_CelingHeight(_vector Pos, _uint iCheckRange)
{
	_int index = PosToIndex(Pos);
	if (index < 0)
		return FLT_MAX;
	_uint iYIdx = index / (m_vSize.x * m_vSize.z);
	_uint iXIdx = index % m_vSize.x;
	_uint iZIdx = index / m_vSize.x % m_vSize.z;

	for (_int yIdx = (_int)iYIdx+1; yIdx < (_int)m_vSize.y; yIdx++)
	{
		_uint iTmpIndex = iXIdx + m_vSize.x * iZIdx + m_vSize.x * m_vSize.z * yIdx;
		if (m_vecCubes[iTmpIndex] != nullptr)
		{
			return m_vecCubes[iTmpIndex]->Get_BottomHeight(Pos);
		}

	}
	return FLT_MAX;
}

void CTerrain::Get_AdjWayFinderCells(_uint Index, vector<_uint>& vecAdjCells)
{
	_uint iCellCount = (_uint)m_vecCubes.size();
	assert(Index < iCellCount);
	XMUINT3 vSplitIdx = SplitIndex(Index);

	_uint iAdjIdx = iCellCount;
	_uint iXAdjIdx = iCellCount;
	_uint iZAdjIdx = iCellCount;
	_uint iYAdjIdx = iCellCount;

	for (_int iDy = -1; iDy <= 1; iDy++)
	{
		for (_int iDz = -1; iDz <= 1; iDz++)
		{
			for (_int iDx = -1; iDx <= 1; iDx++)
			{
				if (iDx == 0 && iDz == 0 && iDy == 0) continue;
				iXAdjIdx = vSplitIdx.x + iDx;
				iZAdjIdx = vSplitIdx.z + iDz;
				iYAdjIdx = vSplitIdx.y + iDy;
				if (iXAdjIdx < 0 || iXAdjIdx >= m_vSize.x) continue;
				if (iZAdjIdx < 0 || iZAdjIdx >= m_vSize.z) continue;
				if (iYAdjIdx < 1 || iYAdjIdx >= m_vSize.y) continue;
				iAdjIdx = iXAdjIdx + m_vSize.x * iZAdjIdx + m_vSize.x * m_vSize.z * iYAdjIdx;
				if (iAdjIdx >= iCellCount) continue;
				//비어있다로 확인하면 안됨. if (m_vecCells[iAdjIdx] != nullptr) continue;
				if (m_vecCubes[iAdjIdx] != nullptr) 
					if (m_vecCubes[iAdjIdx]->Is_BlockingType())
						continue;
				_uint iFloorIdx = iXAdjIdx + m_vSize.x * iZAdjIdx + m_vSize.x * m_vSize.z * (iYAdjIdx - 1);
				if (m_vecCubes[iFloorIdx] == nullptr) continue;
				if (false == m_vecCubes[iFloorIdx]->Is_BlockingType()) continue;
				vecAdjCells.push_back(iAdjIdx);
			}
		}
	}
}

void CTerrain::Get_XZAdjWayFinderCells(_uint Index, vector<_uint>& vecAdjCells)
{
	_uint iCellCount = (_uint)m_vecCubes.size();
	assert(Index < iCellCount);
	XMUINT3 vSplitIdx = SplitIndex(Index);

	_uint iAdjIdx = iCellCount;
	_uint iXAdjIdx = iCellCount;
	_uint iZAdjIdx = iCellCount;

	for (_int iDz = -1; iDz <= 1; iDz++)
	{
		for (_int iDx = -1; iDx <= 1; iDx++)
		{
			if (iDx == 0 && iDz == 0 ) continue;
			iXAdjIdx = vSplitIdx.x + iDx;
			iZAdjIdx = vSplitIdx.z + iDz;
			if (iXAdjIdx < 0 || iXAdjIdx >= m_vSize.x) continue;
			if (iZAdjIdx < 0 || iZAdjIdx >= m_vSize.z) continue;
			iAdjIdx = iXAdjIdx + m_vSize.x * iZAdjIdx + m_vSize.x * m_vSize.z * vSplitIdx.y;
			if (iAdjIdx >= iCellCount) continue;
			//비어있다로 확인하면 안됨. if (m_vecCells[iAdjIdx] != nullptr) continue;
			if (m_vecCubes[iAdjIdx] != nullptr)
				if (m_vecCubes[iAdjIdx]->Is_BlockingType())
					continue;
			_uint iFloorIdx = iXAdjIdx + m_vSize.x * iZAdjIdx + m_vSize.x * m_vSize.z * (vSplitIdx.y - 1);
			if (m_vecCubes[iFloorIdx] == nullptr) continue;
			if (false == m_vecCubes[iFloorIdx]->Is_BlockingType()) continue;
			vecAdjCells.push_back(iAdjIdx);
		}
	}
	
}

_float CTerrain::Get_Distance(_uint StartIndex, _uint DestIndex)
{
	return XMVector4Length(IndexToPos(DestIndex) - IndexToPos(StartIndex)).m128_f32[0] ;
}

_float CTerrain::Get_AdjDistance(_uint StartIndex, _uint DestIndex)
{
	CELL_RELATION eRelation = Get_AdjCell_Relation(StartIndex, DestIndex);
	switch (eRelation)
	{
	case CELL_RELATION::STRAIGHT:
		return STRAIGHT_DIST;
	case CELL_RELATION::DIAG:
		return DIAG_DIST;
	case CELL_RELATION::TRIAG:
		return TRIAG_DIST;
	default:
		return -1;
	}
}

CTerrain::CELL_RELATION CTerrain::Get_AdjCell_Relation(_uint StartIndex, _uint DestIndex)
{
	_uint iXStart = StartIndex % m_vSize.x;
	_uint iZStart = StartIndex / m_vSize.x % m_vSize.z;
	_uint iYStart = StartIndex / (m_vSize.x * m_vSize.z);

	_uint iXDest = DestIndex % m_vSize.x;
	_uint iZDest = DestIndex / m_vSize.x % m_vSize.z;
	_uint iYDest = DestIndex / (m_vSize.x * m_vSize.z);

	_int iDiff = (_int)iXDest - (_int)iXStart;
	_int iDist = iDiff > 0 ? iDiff : -iDiff;
	iDiff = (_int)iZDest - (_int)iZStart;
	iDist += iDiff > 0 ? iDiff : -iDiff;
	iDiff = (_int)iYDest - (_int)iYStart;
	iDist += iDiff > 0 ? iDiff : -iDiff;

	switch (iDist)
	{
	case 0:
		return CELL_RELATION::SAME;
	case 1:
		return CELL_RELATION::STRAIGHT;
	case 2:
		return CELL_RELATION::DIAG;
	case 3:
		return CELL_RELATION::TRIAG;
	default:
		assert(false);
		return CELL_RELATION::LAST;
	}
	
}

CTerrainObject* CTerrain::Get_Portal(LEVELID eLinkedLevel)
{
	for (auto& pCell : m_pChilds)
	{
		if (nullptr == pCell) continue;
		CPortalTerrainObject* pTerrObj = static_cast<CPortalTerrainObject*>(pCell);
		if (pTerrObj->Get_BuildItemType() == BUILD_ITEM_TYPE::PORTAL)
			if (pTerrObj->Get_LinkedLevelID() == eLinkedLevel)
				return pTerrObj;
	}
	return nullptr;
}

CTerrainObject* CTerrain::Get_PlayerSpawn()
{
	for (auto& pCell : m_pChilds)
	{
		if (nullptr == pCell) continue;
		CPortalTerrainObject* pTerrObj = static_cast<CPortalTerrainObject*>(pCell);
		if (pTerrObj->Get_BuildItemType() == BUILD_ITEM_TYPE::PLAYER_SPAWNER)
			return pTerrObj;
	}
	return nullptr;
}

_vector CTerrain::Get_ContainedCellPosition(const _fvector& Pos)
{
	if (false == Is_InSide(Pos))
		return {-1,-1,-1,-1};

	_int iIdx = PosToIndex(Pos);
	return IndexToPos(iIdx);
}

void CTerrain::Get_ContainingCells(CColliderBase* pCollider, list<_uint>& vecCells)
{
	_uint iCellCount =(_uint) m_vecCubes.size();
	for (_uint i = 0; i < iCellCount; i++)
	{
		_vector vPos = IndexToPos(i);
		vPos.m128_f32[1] += 0.5;
		if (pCollider->Contains(vPos))
			vecCells.push_back(i);
	}
}





_bool CTerrain::Is_Buildable(_vector Pos)
{
	if (false == Is_InSide(Pos))
		return false;
	_int iIdx = PosToIndex(Pos);
	if (false  == Is_ValidIndex(iIdx))
		return false;
	if (m_vecCubes[iIdx] != nullptr)
		return false;

	return true;
}

_bool CTerrain::Is_InSide(_vector vPos)
{
	_float4 fPos;
	XMStoreFloat4(&fPos, vPos);
	if (fPos.x < -0.5 || fPos.x >= m_vSize.x - 0.5)
		return false;
	if (fPos.z < -0.5 || fPos.z >= m_vSize.z - 0.5)
		return false;
	if (fPos.y < 0 || fPos.y >= m_vSize.y )
		return false;
	return true;
}

_bool CTerrain::Is_ValidIndex(_uint iIndex)
{
	return iIndex >= 0 && iIndex < m_vecCubes.size();
}

_bool CTerrain::Is_ValidIndex(XMUINT3 i3Index)
{
	if (i3Index.x < 0 || i3Index.x >= m_vSize.x)
		return false;
	if (i3Index.y < 0 || i3Index.y >= m_vSize.y)
		return false;
	if (i3Index.z < 0 || i3Index.z >= m_vSize.z)
		return false;
	return true;
}


CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const char* szMapFileName)
{
	CTerrain*	pInstance = new CTerrain(pDevice, pContext, szMapFileName);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCubeTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	for (auto& pCube : m_vecCubes)
	{
		Safe_Release(pCube);
	}
	m_vecCubes.clear();

	Safe_Release(m_pOctoTree);
	__super::Free();
}

CTerrainObject* TerrainObjectFactory::Create(BUILD_ITEM_TYPE eType, CGameInstance* pGI, CTerrainObject::TERRAINOBJ_DESC& tDesc)
{
	CTerrainObject* pGameObject = nullptr;
	switch (eType)
	{
	case Client::BUILD_ITEM_TYPE::GROUND:
	case Client::BUILD_ITEM_TYPE::CUBRIC:
	case Client::BUILD_ITEM_TYPE::FUNCT:
	case Client::BUILD_ITEM_TYPE::DEFORM:
	case Client::BUILD_ITEM_TYPE::FLOOR:
	case Client::BUILD_ITEM_TYPE::WALL:
	case Client::BUILD_ITEM_TYPE::NATURE:
	case Client::BUILD_ITEM_TYPE::PROP:
	case Client::BUILD_ITEM_TYPE::STRUC:
		pGameObject = static_cast<CTerrainObject*>(pGI->Clone_Proto_Object_Stock(CTerrainObject::m_szProtoTag, &tDesc));
		break;
	case Client::BUILD_ITEM_TYPE::MONSTER_SPAWNER:
		pGameObject = static_cast<CTerrainObject*>(pGI->Clone_Proto_Object_Stock(CMonsterSpawner::m_szProtoTag, &tDesc));
		break;
	case Client::BUILD_ITEM_TYPE::PLAYER_SPAWNER:
		break;
	case Client::BUILD_ITEM_TYPE::PORTAL:
		pGameObject = static_cast<CTerrainObject*>(pGI->Clone_Proto_Object_Stock(CPortalTerrainObject::m_szProtoTag, &tDesc));
		break;
	case Client::BUILD_ITEM_TYPE::NPC_SPAWNER:
		pGameObject = static_cast<CTerrainObject*>(pGI->Clone_Proto_Object_Stock(CNPCSpanwer::m_szProtoTag, &tDesc));
		break;
	case Client::BUILD_ITEM_TYPE::LAST:
	default:
		break;
	}

	return pGameObject;
}

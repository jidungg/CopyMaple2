#include "stdafx.h"
#include "..\Public\CubeTerrain.h"

#include "GameInstance.h"
#include "JsonParser.h"
#include "ItemDataBase.h"
#include "Character.h"
#include "MonsterSpawner.h"
#include "PortalTerrainObject.h"

CCubeTerrain::CCubeTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const char* szMapFileName)
	: CGameObject { pDevice, pContext }
{
	m_strJsonFilePath = szMapFileName;
}

CCubeTerrain::CCubeTerrain(const CCubeTerrain & Prototype)
	: CGameObject { Prototype },
	m_strJsonFilePath{ Prototype.m_strJsonFilePath }
{
}

HRESULT CCubeTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCubeTerrain::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Load_From_Json(m_strJsonFilePath)))
		return E_FAIL;
	return S_OK;
}


void CCubeTerrain::Late_Update(_float fTimeDelta)
{
	//__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}




HRESULT CCubeTerrain::Load_From_Json(string strJsonFilePath)
{
	CItemDataBase* pDB = ITEMDB;
	json j;
	if (FAILED(CJsonParser::ReadJsonFile(strJsonFilePath.c_str(), &j)))
		return E_FAIL;

	m_vSize = { j["size"][0],j["size"][1],j["size"][2] };
	m_vecCells.resize(m_vSize.x * m_vSize.y * m_vSize.z, nullptr);
	for (const auto& item : j["cells"]) {
		BUILD_ITEM_ID eId =  item["ItemId"];
		size_t iteration = item["Iteration"];
		_uint terrIdx = item["Index"];
		BUILD_ITEM_DATA* pBuildItemDesc = static_cast<BUILD_ITEM_DATA*>( pDB->Get_Data(ITEM_TYPE::BUILD,(_uint)eId));

		CTerrainObject::TERRAINOBJ_DESC desc;
		desc.fRotationPerSec = 5.f;
		desc.fSpeedPerSec = 1.f;
		strcpy_s(desc.strModelProtoName, pBuildItemDesc->strModelTag);
		desc.eModelProtoLevelID = LEVEL_LOADING;
		desc.eID = eId;
		desc.direction = item["Direction"];
		desc.vecIData = item["IntData"].get<vector<_int>>();
		desc.vecFData = item["FloatData"].get<vector<_float>>();

		for (int i = 0 ; i < iteration; i++)
		{
			desc.index = terrIdx + i;
			XMStoreFloat4(&desc.pos, IndexToPos(desc.index));
			if(FAILED(Add_TerrainObject(desc)))
				return E_FAIL;
		}
	}
	return S_OK;
}

//MoveDir 는 Normalized 된 상태로 들어와야 함.
_vector CCubeTerrain::BlockXZ(CCharacter* pCharacter)
{
	_vector vPos = pCharacter->Get_Position();
	_vector vMoveDir = pCharacter->Get_MoveDirection();
	_float fBodyCollisionRadius = pCharacter->Get_BodyCollisionRadius();
	_float3 fBodyCollisionOffset = pCharacter->Get_BodyCollisionOffset();
	_float fMoveDistance = pCharacter->Get_MoveDistance();
	_float fXForce = XMVectorGetX(vMoveDir);
	_float fZForce = XMVectorGetZ(vMoveDir);
	_float fYForce = XMVectorGetY(vMoveDir);
	_uint iCheckRange = (_uint)floor(fBodyCollisionRadius + fMoveDistance)+1;
	_int iIdx =  PosToIndex(vPos);
	if (iIdx < 0)
		return  vPos + vMoveDir * fMoveDistance;
	_int iYIdx = iIdx / (m_vSize.x * m_vSize.z);
	_int iXIdx = iIdx % m_vSize.x;
	_int iZIdx = iIdx / m_vSize.x % m_vSize.z;

	_int iMinX = iXIdx, iMaxX = iXIdx, iMinZ = iZIdx, iMaxZ = iZIdx, iMinY = iYIdx, iMaxY = iYIdx;

	if (fXForce > 0)
		iMaxX += iCheckRange;
	else if (fXForce < 0)
		iMinX -= iCheckRange;
	if (fZForce > 0)
		iMaxZ += iCheckRange;
	else if (fZForce < 0)
		iMinZ -= iCheckRange;
	iMaxY += iCheckRange;

	iMaxX = min(iMaxX, m_vSize.x - 1);
	iMinX = max(iMinX, 0);
	iMaxZ = min(iMaxZ, m_vSize.z - 1);
	iMinZ = max(iMinZ, 0);
	iMaxY = min(iMaxY, m_vSize.y - 1);
	iMinY = max(iMinY, 0);

	_vector vNextPos = vPos;
	for (_uint iY = iMinY; iY < iMaxY; iY++)
	{
		for (_uint iZ = iMinZ; iZ <= iMaxZ; iZ++)
		{
			for (_uint iX = iMinX; iX <= iMaxX; iX++)
			{
				_uint iIdx = iX + m_vSize.x * iZ + m_vSize.x * m_vSize.z * iY;
				if (nullptr == m_vecCells[iIdx])
					continue;
				if (false == m_vecCells[iIdx]->Is_BlockingType())
					continue;
				vNextPos = m_vecCells[iIdx]->BolckXZ(vPos, vMoveDir, fMoveDistance, fBodyCollisionRadius, fBodyCollisionOffset.y);
				_vector vTmp = vNextPos - vPos;
				vMoveDir = XMVector4Normalize(vTmp);
				fMoveDistance = XMVectorGetX(XMVector3Length(vTmp));
			}
		}
	}


	vNextPos = vPos + fMoveDistance * vMoveDir;
	return vNextPos;
}


_bool CCubeTerrain::RayCastXZ(const Ray& tRay, RaycastHit* pOut)
{
	_vector vCurrentPos = tRay.vOrigin;
	vCurrentPos += XMVectorSet(0.5, 0, 0.5, 0);
	_float fXDir = XMVectorGetX(tRay.vDirection);
	_float fZDir = XMVectorGetZ(tRay.vDirection);
	//가장 가까운 정수값이 되기 위해 필요한 값 찾기
	_float fX = XMVectorGetX(vCurrentPos);
	_float fZ = XMVectorGetZ(vCurrentPos);
	_float fXRequire = 0;
	_float fZRequire = 0;
	_float fCurrentDistance = 0;

	while (fCurrentDistance <= tRay.fDist)
	{
		if (fXDir > 0)
		{
			fXRequire = ceilf(fX) - fX;
			if (fXRequire == 0)
				fXRequire += 1;
		}
		else if (fXDir < 0)
		{
			fXRequire = floorf(fX) - fX;
			if (fXRequire == 0)
				fXRequire -= 1;
		}
		else
			fXRequire = FLT_MAX;
		if (fZDir > 0)
		{
			fZRequire = ceilf(fZ) - fZ;
			if (fZRequire == 0)
				fZRequire += 1;
		}
		else if (fZDir < 0)
		{
			fZRequire = floorf(fZ) - fZ;
			if (fZRequire == 0)
				fZRequire -= 1;
		}
		else
			fZRequire = FLT_MAX;


		_float fXReqTime = (fXRequire + 1e-6f) / (fXDir + 1e-6f);
		_float fZReqTime = (fZRequire + 1e-6f) / (fZDir + 1e-6f);
		//Z축 이 더 빨리 도달
		if (fXReqTime > fZReqTime)
			vCurrentPos += tRay.vDirection * fZReqTime;
		//X축이 더 빨리 도달
		else
			vCurrentPos += tRay.vDirection * fXReqTime;
		_vector vRealPos = vCurrentPos;
		vRealPos -= XMVectorSet(0.5, 0, 0.5, 0);
		_int iIdx = PosToIndex(vRealPos);
		if (iIdx < 0)
			return false;
		fCurrentDistance = XMVectorGetX(XMVector3Length(vRealPos - tRay.vOrigin));
		if (nullptr != m_vecCells[iIdx] && m_vecCells[iIdx]->RayCast(tRay, pOut))
			return true;

	}


	return false;
}

HRESULT CCubeTerrain::Save_To_Json(string strNewFilepath)
{
	json j;
	j["cells"] = json::array();
	json& jCells = j["cells"];
	json jCurObj;
	bool bFirst = true;
	for (auto& cell : m_vecCells)
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

_vector CCubeTerrain::IndexToPos(_uint Index)
{
	return XMVectorSet(Index % m_vSize.x, 
		Index / (m_vSize.x * m_vSize.z),
		Index / m_vSize.x % m_vSize.z,
		1);
}

_uint CCubeTerrain::PosToIndex(const _float4& Pos)
{
	_uint x = static_cast<_uint>(roundf( Pos.x));
	_uint y = static_cast<_uint>(floorf(Pos.y)); // 0~1 = 0
	_uint z = static_cast<_uint>(roundf(Pos.z));

	return x + m_vSize.x * z + m_vSize.x * m_vSize.z * y;
}

_int CCubeTerrain::PosToIndex(const _fvector& Pos)
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

XMUINT3 CCubeTerrain::SplitIndex(_uint iIdx)
{
	return XMUINT3{ iIdx % m_vSize.x ,iIdx / (m_vSize.x * m_vSize.z),iIdx / m_vSize.x % m_vSize.z, };
}

_uint CCubeTerrain::CombineIndex(XMUINT3 i3Idx)
{
	return i3Idx.x + m_vSize.x * i3Idx.z + m_vSize.x * m_vSize.z * i3Idx.y;
}

HRESULT CCubeTerrain::Add_TerrainObject( CTerrainObject::TERRAINOBJ_DESC& tDesc)
{
	if (m_vecCells[tDesc.index] != nullptr)
		return E_FAIL;
	tDesc.pCubeTerrain = this;
	BUILD_ITEM_TYPE eType = static_cast<BUILD_ITEM_DATA*>(ITEMDB->Get_Data(ITEM_TYPE::BUILD, (_uint)tDesc.eID))->eBuildType;

	CTerrainObject* pGameObject = nullptr;
	switch (eType)
	{
	case Client::BUILD_ITEM_TYPE::CUBE:
	case Client::BUILD_ITEM_TYPE::FILED_BLOCK:
	case Client::BUILD_ITEM_TYPE::FILED_NON_BLOCK:
		pGameObject = static_cast<CTerrainObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CTerrainObject::m_szProtoTag, &tDesc));
		break;
	case Client::BUILD_ITEM_TYPE::INTERACTABLE:
		if (tDesc.eID == Client::BUILD_ITEM_ID::PORTAL)
			pGameObject = static_cast<CTerrainObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CPortalTerrainObject::m_szProtoTag, &tDesc));
		break;
	case Client::BUILD_ITEM_TYPE::SPAWN:
	{
		switch (tDesc.eID)
		{
		case Client::BUILD_ITEM_ID::MONSTER_SPAWNER:

			pGameObject = static_cast<CTerrainObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CMonsterSpawner::m_szProtoTag, &tDesc));
			break;
		case Client::BUILD_ITEM_ID::PLAYER_SPAWNER:
			//pGameObject = static_cast<CTerrainObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CMonsterSpawner::m_szProtoTag, &tDesc));
			break;
		default:
			break;
		}
		break;
	}
	case Client::BUILD_ITEM_TYPE::LAST:
		break;
	default:
		break;
	}
	if(nullptr == pGameObject)
		return E_FAIL;
	Add_Child(pGameObject);
	m_vecCells[tDesc.index] = pGameObject;
	return S_OK;
}

HRESULT CCubeTerrain::Remove_TerrainObject(_uint Index)
{
	if (Index >= m_vecCells.size())
		return E_FAIL;
	if (m_vecCells[Index] == nullptr)
		return E_FAIL;
	m_vecCells[Index]->Set_Dead();
	m_vecCells[Index] = nullptr;

	return S_OK;
}

CTerrainObject* CCubeTerrain::Get_TerrainObject(_uint Index)
{
	if (Index < m_vecCells.size())
		return m_vecCells[Index];
	return nullptr;
}

_float CCubeTerrain::Get_FloorHeight(_vector Pos)
{
	_int index = PosToIndex(Pos);
	if (index < 0)
		return -1;
	_uint iYIdx = index / (m_vSize.x * m_vSize.z);
	_uint iXIdx = index % m_vSize.x;
	_uint iZIdx = index / m_vSize.x % m_vSize.z;
	
	for (_int yIdx = (_int)iYIdx; yIdx >= 0; yIdx--)
	{
		_uint iTmpIndex = iXIdx + m_vSize.x * iZIdx + m_vSize.x * m_vSize.z * yIdx;
		if (m_vecCells[iTmpIndex] != nullptr)
		{
			
			if(m_vecCells[iTmpIndex]->Is_BlockingType())
				return m_vecCells[iTmpIndex]->Get_TopHeight(Pos);
		}
		
	}
	return -1;
}

_float CCubeTerrain::Get_CelingHeight(_vector Pos)
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
		if (m_vecCells[iTmpIndex] != nullptr)
		{
			return m_vecCells[iTmpIndex]->Get_BottomHeight(Pos);
		}

	}
	return FLT_MAX;
}

void CCubeTerrain::Get_AdjCells(_uint Index, vector<_uint>& vecAdjCells)
{
	_uint iCellCount = m_vecCells.size();
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
				if (m_vecCells[iAdjIdx] != nullptr) 
					if (m_vecCells[iAdjIdx]->Is_BlockingType())
						continue;
				_uint iFloorIdx = iXAdjIdx + m_vSize.x * iZAdjIdx + m_vSize.x * m_vSize.z * (iYAdjIdx - 1);
				if (m_vecCells[iFloorIdx] == nullptr) continue;
				if (false == m_vecCells[iFloorIdx]->Is_BlockingType()) continue;
				vecAdjCells.push_back(iAdjIdx);
			}
		}
	}
}

_float CCubeTerrain::Get_Distance(_uint StartIndex, _uint DestIndex)
{
	return XMVector4Length(IndexToPos(DestIndex) - IndexToPos(StartIndex)).m128_f32[0] ;
}

_float CCubeTerrain::Get_AdjDistance(_uint StartIndex, _uint DestIndex)
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

CCubeTerrain::CELL_RELATION CCubeTerrain::Get_AdjCell_Relation(_uint StartIndex, _uint DestIndex)
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

CTerrainObject* CCubeTerrain::Get_Portal(LEVELID eLinkedLevel)
{
	for (auto& pCell : m_pChilds)
	{
		if (nullptr == pCell) continue;
		CPortalTerrainObject* pTerrObj = static_cast<CPortalTerrainObject*>(pCell);
		if (pTerrObj->Get_BuildItemID() == BUILD_ITEM_ID::PORTAL)
			if (pTerrObj->Get_LinkedLevelID() == eLinkedLevel)
				return pTerrObj;
	}
	return nullptr;
}

CTerrainObject* CCubeTerrain::Get_PlayerSpawn()
{
	for (auto& pCell : m_pChilds)
	{
		if (nullptr == pCell) continue;
		CPortalTerrainObject* pTerrObj = static_cast<CPortalTerrainObject*>(pCell);
		if (pTerrObj->Get_BuildItemID() == BUILD_ITEM_ID::PLAYER_SPAWNER)
			return pTerrObj;
	}
	return nullptr;
}

_vector CCubeTerrain::Get_ContainedCellPosition(const _fvector& Pos)
{
	if (false == Is_InSide(Pos))
		return {-1,-1,-1,-1};

	_int iIdx = PosToIndex(Pos);
	return IndexToPos(iIdx);
}





_bool CCubeTerrain::Is_InSide(_vector vPos)
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

_bool CCubeTerrain::Is_ValidIndex(XMUINT3 i3Index)
{
	if (i3Index.x < 0 || i3Index.x >= m_vSize.x)
		return false;
	if (i3Index.y < 0 || i3Index.y >= m_vSize.y)
		return false;
	if (i3Index.z < 0 || i3Index.z >= m_vSize.z)
		return false;
	return true;
}


CCubeTerrain * CCubeTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const char* szMapFileName)
{
	CCubeTerrain*	pInstance = new CCubeTerrain(pDevice, pContext, szMapFileName);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCubeTerrain::Clone(void * pArg)
{
	CCubeTerrain* pInstance = new CCubeTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCubeTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCubeTerrain::Free()
{
	__super::Free();
}

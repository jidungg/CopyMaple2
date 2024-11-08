#include "stdafx.h"
#include "..\Public\CubeTerrain.h"

#include "GameInstance.h"
#include "JsonParser.h"
#include "ItemDataBase.h"
#include "Character.h"

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

	CTerrainObject::TERRAINOBJ_DESC desc;
	desc.fRotationPerSec = 5.f;
	desc.fSpeedPerSec = 1.f;
	for (const auto& item : j["cells"]) {
		BUILD_ITEM_ID eId =  item["ItemId"];
		BUILD_ITEM_DATA* pBuildItemDesc = static_cast<BUILD_ITEM_DATA*>( pDB->Get_Data(ITEM_TYPE::BUILD,(_uint)eId));
		strcpy_s(desc.strModelProtoName, pBuildItemDesc->strModelTag);
		desc.eModelProtoLevelID = LEVEL_LOADING;
		desc.eID = item["ItemId"];
		desc.direction = item["Direction"];
		desc.data = item["Data"];
		size_t iteration = item["Iteration"];
		_uint terrIdx = item["Index"];

		for (int i = 0 ; i < iteration; i++)
		{
			desc.index = terrIdx + i;
			desc.pos = IndexToPos(desc.index);
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
	for (auto& cell : m_pChilds)
	{
		if (cell == nullptr) continue;
		CTerrainObject* pTerrainObj = static_cast<CTerrainObject*>(cell);
		if (bFirst)
		{
			jCurObj = pTerrainObj->ToJson();
			jCells.push_back(jCurObj);
			bFirst = false;
			continue;
		}
		json jTemp = pTerrainObj->ToJson();
		int curIdx = jCurObj["Index"];
		int curIter = jCurObj["Iteration"];
		if ( jCurObj["ItemId"] == jTemp["ItemId"]
			&& jCurObj["Data"] == jTemp["Data"]
			&& jCurObj["Direction"] == jCurObj["Direction"]
			&& (curIdx +curIter) == jTemp["Index"])
		{
			jCells.back()["Iteration"] = jCells.back()["Iteration"] + 1;
		}
		else
		{
			jCells.push_back(jTemp);
			jCurObj = jTemp;

		}

	}
	j["size"] = { m_vSize.x,m_vSize.y,m_vSize.z };

	CJsonParser::SaveJsonFile(strNewFilepath.c_str(),j);
	return S_OK;
}

_float4 CCubeTerrain::IndexToPos(_uint Index)
{
	_float4 pos;
	pos.x = Index % m_vSize.x;
	pos.z = Index / m_vSize.x % m_vSize.z;
	pos.y = Index / (m_vSize.x * m_vSize.z);
	pos.w = 1;
	return pos;
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

HRESULT CCubeTerrain::Add_TerrainObject( CTerrainObject::TERRAINOBJ_DESC& tDesc)
{
	if (m_vecCells[tDesc.index] != nullptr)
		return E_FAIL;
	CTerrainObject* pGameObject = static_cast<CTerrainObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CTerrainObject::m_szProtoTag, &tDesc));
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

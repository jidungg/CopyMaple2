#include "stdafx.h"
#include "..\Public\CubeTerrain.h"

#include "GameInstance.h"
#include "JsonParser.h"
#include "ItemDataBase.h"

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
	json j;
	if (FAILED(CJsonParser::ReadJsonFile(strJsonFilePath.c_str(), &j)))
		return E_FAIL;

	m_vSize = { j["size"][0],j["size"][1],j["size"][2] };
	m_vecCells.resize(m_vSize.x * m_vSize.y * m_vSize.z, nullptr);

	CTerrainObject::TERRAINOBJ_DESC desc;
	desc.fRotationPerSec = 5.f;
	desc.fSpeedPerSec = 1.f;
	for (const auto& item : j["cells"]) {

		string str= item["model"];
		strncpy_s(desc.strItemName, str.c_str(), sizeof(desc.strItemName));
		desc.strItemName[sizeof(desc.strItemName) - 1] = '\0';
		CItemDataBase* pDB = ITEMDB;
		ITEM_DESC* itemdesc = pDB->GetItemDesc(ITEM_TYPE::BUILD,desc.strItemName);
		strcpy_s(desc.strModelProtoName, itemdesc->strModelTag);
		desc.eModelProtoLevelID = LEVEL_LOADING;
		//strcpy_s( desc.strShaderProtoName ,("Prototype_Component_Shader_VtxMesh"));
		//desc.eShaderProtoLevelID = LEVEL_LOADING;
		desc.eType = item["type"];
		desc.direction = item["direction"];
		desc.data = item["data"];
		size_t iteration = item["iteration"];
		_uint terrIdx = item["index"];

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
		int curIdx = jCurObj["index"];
		int curIter = jCurObj["iteration"];
		if ( jCurObj["type"] == jTemp["type"]
			&& jCurObj["model"] == jTemp["model"]
			&& jCurObj["data"] == jTemp["data"]
			&& jCurObj["direction"] == jCurObj["direction"]
			&& (curIdx +curIter) == jTemp["index"])
		{
			jCells.back()["iteration"] = jCells.back()["iteration"] + 1;
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

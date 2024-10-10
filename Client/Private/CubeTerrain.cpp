#include "stdafx.h"
#include "..\Public\CubeTerrain.h"

#include "GameInstance.h"
#include "JsonParser.h"
#include "TerrainObject.h"

CCubeTerrain::CCubeTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* szMapFileName)
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
	if (FAILED(Save_To_Json(m_strJsonFilePath)))
		return E_FAIL;
	return S_OK;
}


void CCubeTerrain::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CCubeTerrain::Render()
{

	for (auto& child : m_pChilds)
	{
		if (child != nullptr && child->Is_Active())
			child->Render();
	}
	return S_OK;
}



HRESULT CCubeTerrain::Load_From_Json(wstring strJsonFilePath)
{
	std::string str(strJsonFilePath.begin(), strJsonFilePath.end());
	json j;
	if (FAILED(CJsonParser::ReadJsonFile(str.c_str(), &j)))
		return E_FAIL;

	m_vSize = { j["size"][0],j["size"][1],j["size"][2] };
	m_pChilds.resize(m_vSize.x * m_vSize.y * m_vSize.z, nullptr);

	json pos, rot, scale;
	for (const auto& item : j["cells"]) {
		pos = item["position"];
		rot = item["rotation"];
		TERRAIN_OBJ_TYPE eType = item["type"];

		int index;
		size_t iteration = item["iteration"];
		CTerrainObject::TERRAINOBJ_DESC desc;


		desc.eType = eType;
		desc.modleName = item["model"];
		desc.Pos = { pos[0],pos[1],pos[2] };
		desc.Rot = { rot[0],rot[1],rot[2] };
		desc.data = item["data"];
		index = desc.Pos.x + desc.Pos.z * m_vSize.x + desc.Pos.y * m_vSize.x * m_vSize.z;
		for (size_t i = 0; i < iteration; i++)
		{
			if (m_pChilds[index + i] != nullptr)
				continue;
			wstring wstr(desc.modleName.begin(), desc.modleName.end());
			CGameObject* pGameObject = static_cast<CGameObject*>(m_pGameInstance->Clone_Proto_Object_Stock(TEXT("Prototype_GameObject_TempTerrainObj"), &desc));

			m_pChilds[index + i] = pGameObject;
		}
	}

	return S_OK;
}

HRESULT CCubeTerrain::Save_To_Json(wstring strNewFilepath)
{
	json j;
	j["cells"] = json::array();
	json& jCells = j["cells"];
	for (auto& cell : m_pChilds)
	{
		if (cell == nullptr) continue;
		CTerrainObject* pTerrainObj = static_cast<CTerrainObject*>(cell);
		jCells.push_back(pTerrainObj->ToJson());

	}
	j["size"] = { m_vSize.x,m_vSize.y,m_vSize.z };

	std::string str(strNewFilepath.begin(), strNewFilepath.end());
	CJsonParser::SaveJsonFile(str.c_str(),j);
	return S_OK;
}


CCubeTerrain * CCubeTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* szMapFileName)
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

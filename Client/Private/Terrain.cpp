#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"
#include "JsonParser.h"


CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* szMapFileName)
	: CGameObject { pDevice, pContext }
{
	m_strMapName = szMapFileName;
}

CTerrain::CTerrain(const CTerrain & Prototype)
	: CGameObject { Prototype },
	m_strMapName{ Prototype.m_strMapName }
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
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;
	if (FAILED(Ready_Cells()))
		return E_FAIL;
	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CTerrain::Update(_float fTimeDelta)
{	

}

void CTerrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	for (auto& child : m_pChilds)
	{
		if (child != nullptr)
			child->Render();
	}
	return S_OK;
}

HRESULT CTerrain::Ready_Cells()
{
	std::string str(m_strMapName.begin(), m_strMapName.end());
	json j;
	CJsonParser::ReadJsonFile(str.c_str(),&j );
	_float3 Size = { j["size"][0],j["size"][1],j["size"][2] };
	m_pChilds.resize(Size.x * Size.y * Size.z, nullptr);

	for (const auto& item : j["cells"]) {
		string modleName = item["model"];
		_float3 Pos = { item["position"][0],item["position"][1],item["position"][2] };
		_float3 Rot = { item["rotation"][0],item["rotation"][1],item["rotation"][2] };
		size_t iteration = item["iteration"];

		int index = Pos.x + Pos.z * Size.x + Pos.y * Size.x * Size.z;
		//TODO : 읽어온 위치, 회전값을	이용하여 셀에 오브젝트를 생성한다.
		//for (size_t i = 0; i < iteration; i++)
		//	m_pChilds[index + i] = m_pGameInstance->Clone_Prototype();
	}

	return S_OK;
}




CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* szMapFileName)
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
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();
}

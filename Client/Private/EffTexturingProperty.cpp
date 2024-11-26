#include "stdafx.h"
#include "EffTexturingProperty.h"
#include "EffTexture.h"

CEffTexturingProperty::CEffTexturingProperty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CEffTexturingProperty::CEffTexturingProperty(const CEffTexturingProperty& rhs)
	:CComponent(rhs)
	, m_vecTexture(rhs.m_vecTexture)
{
	for (auto& pTexture : m_vecTexture)
		Safe_AddRef(pTexture);
}

HRESULT CEffTexturingProperty::Initialize_Prototype(const _char* szDirPath, ifstream& inFile)
{
	_uint iNumTexture = 0;
	inFile.read(reinterpret_cast<char*>(&iNumTexture), sizeof(_uint));

	m_vecTexture.resize(EFF_TEX_TYPE::TT_LAST, nullptr);
	for (_uint iTexIndex = 0; iTexIndex < iNumTexture; iTexIndex++)
	{
		EFF_TEX_TYPE eTexType = EFF_TEX_TYPE::TT_LAST;
		inFile.read(reinterpret_cast<char*>(&eTexType), sizeof(_uint));
		CEffTexture* pTexture = CEffTexture::Create(m_pDevice, m_pContext, szDirPath, inFile, eTexType);
		m_vecTexture[eTexType] = pTexture;
	}
	return S_OK;
}

void CEffTexturingProperty::Update_Value(vector<class CEffController*>* pControllerList)
{
}

HRESULT CEffTexturingProperty::Bind_Texture(CShader* pShader, const _char* pConstantName, EFF_TEX_TYPE eType, _uint iTextureIndex)
{
	if (m_vecTexture[(_uint)eType] == nullptr)
		return E_FAIL;
	return m_vecTexture[(_uint)eType]->Bind_ShaderResource(pShader, pConstantName, iTextureIndex);
}

CEffTexturingProperty* CEffTexturingProperty::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, ifstream& inFile)
{
	CEffTexturingProperty* pInstance = new CEffTexturingProperty(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szDirPath, inFile)))
	{
		MSG_BOX("Failed to Created : CEffTexturingProperty");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CEffTexturingProperty::Clone(void* pArg)
{
	return  new CEffTexturingProperty(*this);
}

void CEffTexturingProperty::Free()
{
	__super::Free();
	for (auto& tex : m_vecTexture)
		Safe_Release(tex);
	m_vecTexture.clear();
}

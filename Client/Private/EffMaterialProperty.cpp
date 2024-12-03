#include "stdafx.h"
#include "EffMaterialProperty.h"
#include "Shader.h"

CEffMaterialProperty::CEffMaterialProperty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CEffMaterialProperty::CEffMaterialProperty(const CEffMaterialProperty& other)
	:CComponent(other)
	, m_tMaterialData(other.m_tMaterialData)
	, m_tDefaultMaterialData(other.m_tDefaultMaterialData)
{
}

HRESULT CEffMaterialProperty::Initialize_Prototype( ifstream& inFile)
{
	inFile.read(reinterpret_cast<char*>(&m_tDefaultMaterialData.vAmbient), sizeof(_float3));
	inFile.read(reinterpret_cast<char*>(&m_tDefaultMaterialData.vDiffuse), sizeof(_float3));
	inFile.read(reinterpret_cast<char*>(&m_tDefaultMaterialData.vSpecular), sizeof(_float3));
	inFile.read(reinterpret_cast<char*>(&m_tDefaultMaterialData.vEmissive), sizeof(_float3));
	inFile.read(reinterpret_cast<char*>(&m_tDefaultMaterialData.fGlossiness), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_tDefaultMaterialData.fAlpha), sizeof(_float));
	m_tMaterialData = m_tDefaultMaterialData;
	return S_OK;
}

HRESULT CEffMaterialProperty::Bind_Material(CShader* pShader)
{
	if (FAILED(pShader->Bind_RawValue("g_vMaterialDiffuse", &m_tMaterialData.vDiffuse, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vMaterialAmbient", &m_tMaterialData.vAmbient, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vMaterialSpecular", &m_tMaterialData.vSpecular, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vMaterialEmissive", &m_tMaterialData.vSpecular, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_fMaterialGlossiness", &m_tMaterialData.fGlossiness, sizeof(_float))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_fMaterialAlpha", &m_tMaterialData.fAlpha, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

void CEffMaterialProperty::Set_Alpha(_float fAlpha)
{
	m_tMaterialData.fAlpha = fAlpha;
}





CEffMaterialProperty* CEffMaterialProperty::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& inFile)
{
	CEffMaterialProperty* pInstance = new CEffMaterialProperty(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype( inFile)))
	{
		MSG_BOX("Failed to Created : CEffMaterialProperty");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CEffMaterialProperty::Clone(void* pArg)
{
	return  new CEffMaterialProperty(*this);
}

void CEffMaterialProperty::Free()
{
	__super::Free();
}

void CEffMaterialProperty::Reset()
{
	m_tMaterialData = m_tDefaultMaterialData;
}

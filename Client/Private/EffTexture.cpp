#include "stdafx.h"
#include "EffTexture.h"
#include "Shader.h"

CEffTexture::CEffTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTexture(pDevice, pContext)
{
}

CEffTexture::CEffTexture(const CEffTexture& Prototype)
	:CTexture(Prototype)
	, m_bHasTextureTransform(Prototype.m_bHasTextureTransform)
	, m_f2Translation(Prototype.m_f2Translation)
	, m_f2Scale(Prototype.m_f2Scale)
	, m_fRotation(Prototype.m_fRotation)
	, m_f2Center(Prototype.m_f2Center)
{
}

HRESULT CEffTexture::Initialize_Prototype(const _char* szDirPath, ifstream& inFIle)
{
	if (FAILED(__super::Initialize_Prototype(szDirPath, inFIle)))
		return E_FAIL;

	inFIle.read(reinterpret_cast<char*>(&m_bHasTextureTransform), sizeof(bool));
	inFIle.read(reinterpret_cast<char*>(&m_f2Translation), sizeof(_float2));
	inFIle.read(reinterpret_cast<char*>(&m_f2Scale), sizeof(_float2));
	inFIle.read(reinterpret_cast<char*>(&m_fRotation), sizeof(_float));
	inFIle.read(reinterpret_cast<char*>(&m_f2Center), sizeof(_float2));

	return S_OK;
}



CEffTexture* CEffTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, ifstream& inFIle, EFF_TEX_TYPE eTexType)
{
	CEffTexture* pInstance = new CEffTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szDirPath, inFIle)))
	{
		MSG_BOX("Failed to Created : CEffTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CEffTexture::Clone(void* pArg)
{
	CEffTexture* pInstance = new CEffTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffTexture::Free()
{
	__super::Free();
}

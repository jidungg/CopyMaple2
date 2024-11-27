#include "stdafx.h"
#include "EffTexture.h"
#include "Shader.h"
#include "EffConcreteController.h"

CEffTexture::CEffTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTexture(pDevice, pContext)
{
}

CEffTexture::CEffTexture(const CEffTexture& Prototype)
	:CTexture(Prototype)
	, m_bHasTextureTransform(Prototype.m_bHasTextureTransform)
	, m_tDefaultTexTransformData(Prototype.m_tDefaultTexTransformData)
{
	m_tTexTransformData = m_tDefaultTexTransformData;
}

HRESULT CEffTexture::Initialize_Prototype(const _char* szDirPath, ifstream& inFIle)
{
	if (FAILED(__super::Initialize_Prototype(szDirPath, inFIle)))
		return E_FAIL;
	inFIle.read(reinterpret_cast<char*>(&m_bHasTextureTransform), sizeof(bool));
	inFIle.read(reinterpret_cast<char*>(&m_tDefaultTexTransformData.f2Translation), sizeof(_float2));
	inFIle.read(reinterpret_cast<char*>(&m_tDefaultTexTransformData.f2Scale), sizeof(_float2));
	inFIle.read(reinterpret_cast<char*>(&m_tDefaultTexTransformData.fRotation), sizeof(_float));
	inFIle.read(reinterpret_cast<char*>(&m_tDefaultTexTransformData.f2Center), sizeof(_float2));

	m_tTexTransformData = m_tDefaultTexTransformData;
	return S_OK;
}

void CEffTexture::Reset()
{
	m_tTexTransformData = m_tDefaultTexTransformData;
}

void CEffTexture::Set_TextureTransformData(EFF_TEX_OPERATION_TYPE eOpType, _float fValue)
{
	switch (eOpType)
	{
	case Client::TO_TRANSLATE_U:
		m_tTexTransformData.f2Translation.x = fValue;
		break;
	case Client::TO_TRANSLATE_V:
		m_tTexTransformData.f2Translation.y = fValue;
		break;
	case Client::TO_ROTATE:
		m_tTexTransformData.fRotation = fValue;
		break;
	case Client::TO_SCALE_U:
		m_tTexTransformData.f2Scale.x = fValue;
		break;
	case Client::TO_SCALE_V:
		m_tTexTransformData.f2Scale.y = fValue;
		break;
	case Client::TO_LAST:
	default:
		break;
	}
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

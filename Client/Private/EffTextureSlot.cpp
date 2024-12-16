#include "stdafx.h"
#include "EffTextureSlot.h"
#include "Shader.h"
#include "EffConcreteController.h"
#include "Texture.h"

CEffTextureSlot::CEffTextureSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CEffTextureSlot::CEffTextureSlot(const CEffTextureSlot& Prototype)
	:CComponent(Prototype)
	, m_bHasTextureTransform(Prototype.m_bHasTextureTransform)
	, m_tDefaultTexTransformData(Prototype.m_tDefaultTexTransformData)
	, m_iDefaultTextureIndex(Prototype.m_iDefaultTextureIndex)
{
	m_tTexTransformData = m_tDefaultTexTransformData;
	 m_iTextureIndex = m_iDefaultTextureIndex;
}



HRESULT CEffTextureSlot::Initialize_Prototype(ifstream& inFIle)
{

	inFIle.read(reinterpret_cast<char*>(&m_iDefaultTextureIndex), sizeof(_uint));
	inFIle.read(reinterpret_cast<char*>(&m_bHasTextureTransform), sizeof(bool));
	inFIle.read(reinterpret_cast<char*>(&m_tDefaultTexTransformData.f2Translation), sizeof(_float2));
	inFIle.read(reinterpret_cast<char*>(&m_tDefaultTexTransformData.f2Scale), sizeof(_float2));
	inFIle.read(reinterpret_cast<char*>(&m_tDefaultTexTransformData.fRotation), sizeof(_float));
	inFIle.read(reinterpret_cast<char*>(&m_tDefaultTexTransformData.f2Center), sizeof(_float2));

	m_tTexTransformData = m_tDefaultTexTransformData;
	m_iTextureIndex =m_iDefaultTextureIndex ;
	return S_OK;
}

HRESULT CEffTextureSlot::Initialize(void* pArg)
{
	return S_OK;
}

void CEffTextureSlot::Reset()
{
	m_tTexTransformData = m_tDefaultTexTransformData;
}

void CEffTextureSlot::Set_TextureTransformData(EFF_TEX_OPERATION_TYPE eOpType, _float fValue)
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

HRESULT CEffTextureSlot::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, vector<CTexture*>& vecTexture ,_uint iSRVIndex)
{
	if (m_iTextureIndex == UINT_MAX)
		return E_FAIL;
	vecTexture[m_iTextureIndex]->Bind_ShaderResource(pShader, pConstantName,iSRVIndex);
	return S_OK;
}



CEffTextureSlot* CEffTextureSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ifstream& inFIle)
{
	CEffTextureSlot* pInstance = new CEffTextureSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype( inFIle)))
	{
		MSG_BOX("Failed to Created : CEffTextureSlot");
		Safe_Release(pInstance);
	}
	return pInstance;
}
CComponent* CEffTextureSlot::Clone(void* pArg)
{
	CEffTextureSlot* pInstance = new CEffTextureSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffTextureSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffTextureSlot::Free()
{
	__super::Free();
}

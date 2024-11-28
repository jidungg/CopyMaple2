#include "stdafx.h"
#include "EffTexturingProperty.h"
#include "EffTexture.h"
#include "Shader.h"
#include "EffConcreteController.h"

const char* szTextureNames[TT_LAST] = {
	"g_BaseTexture",
	"g_DarkTexture",
	"g_DetailTexture",
	"g_GlossTexture",
	"g_GlowTexture",
	"g_BumpTexture",
	"g_NormalTexture",
	"g_ParallaxTextur",
	"g_Decal0Texture",
	"g_Decal1Texture",
	"g_Decal2Texture",
	"g_Decal3Texture",
};


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
	//for (_uint i = 0; i < TT_LAST; ++i)
	//{
	//	m_pSRV[i] = rhs.m_pSRV[i];
	//	Safe_AddRef(m_pSRV[i]);
	//}
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
	//for (_uint i = 0; i < TT_LAST; ++i)
	//{
	//	if (m_vecTexture[i] == nullptr)
	//		m_pSRV[i] =  CreateEmptySRV();
	//	else
	//		m_pSRV[i] = m_vecTexture[i]->Get_SRV();
	//}
	return S_OK;
}


HRESULT CEffTexturingProperty::Bind_Texture(CShader* pShader)
{
	ZeroMemory(m_f2TexcoordScale, sizeof(_float2) * 12);
	ZeroMemory(m_f2TexcoordRotate, sizeof(_float) * 12);
	ZeroMemory(m_f2TexcoordTranslate, sizeof(_float2) * 12);
	ZeroMemory(m_f2TexcoordCenter, sizeof(_float2) * 12);

	_uint		iNumBones = { 0 };


	for (_uint i = 0; i < TT_LAST; ++i)
	{
		if (m_vecTexture[i] == nullptr)
		{
			TextureTransformData tTexTransformData;

			m_f2TexcoordScale[i] = tTexTransformData.f2Scale;
			m_f2TexcoordRotate[i] = tTexTransformData.fRotation ;
			m_f2TexcoordTranslate[i] = tTexTransformData.f2Translation;
			m_f2TexcoordCenter[i] = tTexTransformData.f2Center;
		}
		else
		{
			const TextureTransformData& tTexTransformData = m_vecTexture[0]->Get_TexTransformData();

			m_f2TexcoordScale[i] = tTexTransformData.f2Scale;
			m_f2TexcoordRotate[i] = tTexTransformData.fRotation + XMConvertToRadians(0);
			m_f2TexcoordTranslate[i] = tTexTransformData.f2Translation;
			m_f2TexcoordCenter[i] = tTexTransformData.f2Center;
		}
	}

	if(FAILED(pShader->Bind_FloatVectorArray("g_vTexcoordScale", m_f2TexcoordScale, 12)))
		return E_FAIL;
	if(FAILED(pShader->Bind_FloatArray("g_fTexcoordRotate", m_f2TexcoordRotate, 12)))
		return E_FAIL;
	if(FAILED(pShader->Bind_FloatVectorArray("g_vTexcoordTranslate", m_f2TexcoordTranslate, 12)))
		return E_FAIL;
	if (FAILED(pShader->Bind_FloatVectorArray("g_vTexcoordCenter", m_f2TexcoordCenter, 12)))
		return E_FAIL;
	//if (FAILED(pShader->Bind_SRVArray("g_TextureArray", m_pSRV,12)))
	//	return E_FAIL;
	//return S_OK;

	for (_uint i = 0; i < TT_LAST; ++i)
	{
		if (m_vecTexture[i] == nullptr)
			continue;
		//const TextureTransformData& tTexTransformData = m_vecTexture[i]->Get_TexTransformData();
		//if (FAILED(pShader->Bind_RawValue("g_vTexcoordScale", &tTexTransformData.f2Scale, sizeof(_float2))))
		//	return E_FAIL;
		//if (FAILED(pShader->Bind_RawValue("g_fTexcoordRotate", &tTexTransformData.fRotation, sizeof(_float))))
		//	return E_FAIL;
		//if (FAILED(pShader->Bind_RawValue("g_vTexcoordTranslate", &tTexTransformData.f2Translation, sizeof(_float2))))
		//	return E_FAIL;
		//if (FAILED(pShader->Bind_RawValue("g_vTexcoordCenter", &tTexTransformData.f2Center, sizeof(_float2))))
		//	return E_FAIL;
		if (FAILED(m_vecTexture[i]->Bind_ShaderResource(pShader, szTextureNames[i], 0)))
			return E_FAIL;
	}

	return S_OK;
}

void CEffTexturingProperty::Set_TextureTransformData(EFF_TEX_TYPE eTexSlot, EFF_TEX_OPERATION_TYPE eOpType, _float fValue)
{
	if (m_vecTexture[eTexSlot] == nullptr)
		return;
	m_vecTexture[eTexSlot]->Set_TextureTransformData(eOpType, fValue);
}

ID3D11ShaderResourceView* CEffTexturingProperty::CreateEmptySRV()
{
	ID3D11Texture2D* pTexture = nullptr;
	ID3D11ShaderResourceView* pSRV = nullptr;

	// 텍스처 설명 초기화
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = 1;                  // 텍스처 너비
	texDesc.Height = 1;                 // 텍스처 높이
	texDesc.MipLevels = 1;              // Mipmap 레벨
	texDesc.ArraySize = 1;              // 배열 크기
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 텍스처 포맷
	texDesc.SampleDesc.Count = 1;       // 멀티샘플링 비활성화
	texDesc.Usage = D3D11_USAGE_DEFAULT; // 기본 사용
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// 초기 데이터 설정 (검정색으로 초기화)
	UINT initData[1] = { 0x00000000 }; // RGBA = (0, 0, 0, 0)
	D3D11_SUBRESOURCE_DATA initSubData = {};
	initSubData.pSysMem = initData;
	initSubData.SysMemPitch = 4; // 1픽셀 * 4바이트(RGBA)

	// 텍스처 생성
	HRESULT hr = m_pDevice->CreateTexture2D(&texDesc, &initSubData, &pTexture);

	// 셰이더 리소스 뷰 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, &pSRV);
	return pSRV;
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
	//for (_uint i = 0; i < TT_LAST; ++i)
	//{
	//	Safe_Release(m_pSRV[i]);
	//}
}

void CEffTexturingProperty::Reset()
{
	for (auto& pTex : m_vecTexture)
		if(pTex) pTex->Reset();

}


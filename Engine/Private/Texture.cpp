#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{
}

CTexture::CTexture(const CTexture & Prototype)
	: CComponent{ Prototype }
	, m_iNumSRVs { Prototype.m_iNumSRVs }
	, m_SRVs { Prototype.m_SRVs }
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const _tchar * pTextureFilePath, _uint iNumTexture)
{
	m_iNumSRVs = iNumTexture;

	_tchar			szTextureFilePath[MAX_PATH] = TEXT("");

	for (size_t i = 0; i < m_iNumSRVs; i++)
	{
		wsprintf(szTextureFilePath, pTextureFilePath, i);

		_tchar		szEXT[MAX_PATH] = TEXT("");

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

		HRESULT		hr = {};

		ID3D11ShaderResourceView*		pSRV = { nullptr };

		if (false == lstrcmpW(szEXT, TEXT(".dds")))
		{
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		else if (false == lstrcmpW(szEXT, TEXT(".tga")))
		{

		}
		else
		{
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSRV);
		}
		

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);
	}


	return S_OK;
}

HRESULT CTexture::Initialize_Prototype(const _char* szDirPath, ifstream& inFIle, TEXTURE_TYPE eTexType)
{
	inFIle.read(reinterpret_cast<char*>(&m_iNumSRVs), sizeof(_uint));
	cout << m_iNumSRVs << endl;
	for (size_t texIdx = 0; texIdx < m_iNumSRVs; texIdx++)
	{
		_uint strLen;
		inFIle.read(reinterpret_cast<char*>(&strLen), sizeof(_uint));
		cout << strLen << endl;
		_char* strTexturePath = new _char[strLen + 1];
		inFIle.read(strTexturePath, sizeof(_char) * strLen);
		strTexturePath[strLen] = '\0';
		cout << strTexturePath << endl;


		_char		szFileName[MAX_PATH] = "";
		_char		szExt[MAX_PATH] = "";

		_char		szFullPath[MAX_PATH] = "";

		_splitpath_s(strTexturePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

		strcat_s(szFullPath, szDirPath);
		strcat_s(szFullPath, szFileName);
		strcat_s(szFullPath, szExt);

		_tchar		szPerfectPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH);
		
		ID3D11ShaderResourceView* pSRV = { nullptr };
		HRESULT		hr = {};
		if (false == strcmp(szExt, ".dds"))
			hr = CreateDDSTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV);
		else if (false == strcmp(szExt, ".tga"))
			return E_FAIL;
		else
			hr = CreateWICTextureFromFile(m_pDevice, szPerfectPath, nullptr, &pSRV);
		if (FAILED(hr))
		{
			CreateDDSTextureFromFile(m_pDevice, TEXT("../Bin/Resources/Textures/Default.dds"), nullptr, &pSRV);
		}
		m_SRVs.push_back(pSRV);
	}
	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader * pShader, const _char * pConstantName, _uint iSRVIndex)
{
	if (iSRVIndex >= m_iNumSRVs)
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_SRVs[iSRVIndex]);	
}

HRESULT CTexture::Push_Texture(ID3D11ShaderResourceView* pSRV)
{
	if (nullptr == pSRV)
		return E_FAIL;

	m_SRVs.push_back(pSRV);

	++m_iNumSRVs;

	return S_OK;
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*	pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}
CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szDirPath, ifstream& inFIle, TEXTURE_TYPE eTexType)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szDirPath, inFIle, eTexType)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}
CComponent * CTexture::Clone(void * pArg)
{
	CTexture*	pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();

}

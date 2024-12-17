#include "..\Public\FontManager.h"


#include "CustomFont.h"
#include "GameInstance.h"


CFontManager::CFontManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGI)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
	m_pGameInstance = pGI;
	Safe_AddRef(m_pGameInstance);
}

HRESULT CFontManager::Load_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	if (nullptr != Find_Font(strFontTag))
		return E_FAIL;

	CCustomFont* pFont = CCustomFont::Create(m_pDevice, m_pContext, pFontFilePath);
	if (nullptr == pFont)
		return E_FAIL;
	
	m_Fonts.emplace(strFontTag, pFont);

	return S_OK;
}

HRESULT CFontManager::Render_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin)
{
	CCustomFont* pFont = Find_Font(strFontTag);
	if (nullptr == pFont)
		return E_FAIL;


	return pFont->Render(pText, vPosition, vColor, fRotation, vOrigin);
}

CCustomFont* CFontManager::Find_Font(const _wstring& strFontTag)
{
	auto	iter = m_Fonts.find(strFontTag);

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

CFontManager* CFontManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGI)
{
	return new CFontManager(pDevice, pContext, pGI);
}

void CFontManager::Free()
{
	__super::Free();

	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);
}

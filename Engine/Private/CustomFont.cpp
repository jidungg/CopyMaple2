#include "..\Public\CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CCustomFont::CCustomFont(const CCustomFont& Prototype)
	: m_pDevice{ Prototype.m_pDevice }
	, m_pContext{ Prototype.m_pContext }
	, m_pFont{ Prototype.m_pFont }
	, m_pBatch{ Prototype.m_pBatch }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

HRESULT CCustomFont::Initialize_Prototype(const _tchar* pFontFilePath)
{
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);

	m_pBatch = new SpriteBatch(m_pContext);

	return S_OK;
}

/* FXMVECTOR color = Colors::White, float rotation = 0, XMFLOAT2 const& origin = Float2Zero, float scale = 1*/
HRESULT CCustomFont::Render(const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin)
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, pText, vPosition, vColor, fRotation, vOrigin);

	m_pBatch->End();

	return S_OK;
}

_float2 CCustomFont::Get_TextSize(const _tchar* pText)
{
	_float2 fSize;
	XMStoreFloat2(&fSize, m_pFont->MeasureString(pText));
	return fSize;
}

CCustomFont* CCustomFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
	CCustomFont* pInstance = new CCustomFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pFontFilePath)))
	{
		MSG_BOX("Failed to Created : CCustomFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCustomFont* CCustomFont::Clone()
{
	return new CCustomFont(*this);
}

void CCustomFont::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Delete(m_pFont);
	Safe_Delete(m_pBatch);
}

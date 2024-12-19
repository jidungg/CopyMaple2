#include "stdafx.h"
#include "UIFont.h"
#include "CustomFont.h"
#include "GameInstance.h"


CUIFont::CUIFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject(pDevice, pContext)
{
}

CUIFont::CUIFont(const CUIFont& Prototype)
	:CUIObject(Prototype)
{

}

HRESULT CUIFont::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	UIFontDesc* pDesc = (UIFontDesc*)pArg;
	m_pFont = m_pGameInstance->Find_Font(pDesc->pFontTag);
	Safe_AddRef(m_pFont);
	m_pText = pDesc->pText;
	m_vColor = pDesc->vColor;
	m_bShade = pDesc->bShade;
	return S_OK;
}
//	m_pGameInstance->Render_Font(TEXT("Font_147"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 1.f));

HRESULT CUIFont::Render()
{
	if (m_pText.empty())
		return S_OK;

	_float2 fOrigin = { 0,0 };
	_float2 fFontPosition = Calc_FontLeftTop(m_pText.c_str());
	if (m_bShade)
	{
		_float2 fPos2 = fFontPosition;
		fPos2.x -= 1.f;
		fPos2.y += 1.f;
		m_pFont->Render(m_pText.c_str(), fPos2, {0,0,0,1}, _float{0}, fOrigin);
	}
	m_pFont->Render(m_pText.c_str(), fFontPosition, m_vColor, _float{ 0 }, fOrigin);
	return S_OK;
}

void CUIFont::Set_Text(const _tchar* pText)
{
	m_pText = pText;
}

_float2 CUIFont::Get_TextSize(const _tchar* pText)
{
	return m_pFont->Get_TextSize(pText);
}

_float2 CUIFont::Calc_FontLeftTop(const _tchar* pText)
{
	CRect_Transform* pTransform = static_cast<CRect_Transform*>(m_pTransformCom);
	CORNOR_TYPE ePivotType = pTransform->Get_PivotType();
	_float2 fPosition = pTransform->Get_PivotPosition(ePivotType);

	_float2 fSize = m_pFont->Get_TextSize(m_pText.c_str());
	_float2		fPivotRatio = pTransform->Get_CornorRatio(ePivotType);
	fPosition.x -= fSize.x *0.5f;
	fPosition.y -= fSize.y *0.5f;
	fPosition.x -= fSize.x * fPivotRatio.x;
	fPosition.y -= fSize.y * fPivotRatio.y;

	return fPosition;
}

bool CUIFont::Check_MouseOver(POINT fPos)
{
	return false;
}
CUIFont* CUIFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIFont* pInstance = new CUIFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIFont");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIFont::Clone(void* pArg)
{
	CUIFont* pInstance = new CUIFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIFont");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIFont::Free()
{
	__super::Free();
	Safe_Release(m_pFont);
}

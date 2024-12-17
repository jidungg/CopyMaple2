#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CFontManager final : public CBase
{
private:
	CFontManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGI);
	virtual ~CFontManager() = default;

public:
	HRESULT Load_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	class CCustomFont* Find_Font(const _wstring& strFontTag);
	HRESULT Render_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f));

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };
	map<const _wstring, class CCustomFont*>			m_Fonts;


public:
	static CFontManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGI);
	virtual void Free() override;
};

END
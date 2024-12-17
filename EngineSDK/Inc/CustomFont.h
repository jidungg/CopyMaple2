#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCustomFont final : public CBase
{
private:
	explicit CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCustomFont(const CCustomFont& Prototype);
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize_Prototype(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f));

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	SpriteFont* m_pFont = { nullptr };
	SpriteBatch* m_pBatch = { nullptr };

public:

	static CCustomFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath);
	virtual CCustomFont* Clone();
	virtual void Free() override;
};

END
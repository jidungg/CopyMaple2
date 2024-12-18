#pragma once
#include "UIObject.h"
BEGIN(Engine)
class CCustomFont;
END
BEGIN(Client)
class CUIFont :
    public CUIObject
{
public:
	typedef struct UIFontDesc : public CUIObject::UIObjectDesc
	{
		const _tchar* pText = { nullptr };
		const _tchar* pFontTag = { nullptr };
		_vector vColor = { 1.f,1.f,1.f };
		_bool bShade = { false };
	} UIFontDesc;

public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIFontDesc";
public:
	virtual bool Check_MouseOver(POINT fPos);
private:
	explicit CUIFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIFont(const CUIFont& Prototype);
	virtual ~CUIFont() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

	void Set_Text(const _tchar* pText);

	_float2 Get_TextSize(const _tchar* pText);
private:
	CCustomFont* m_pFont = { nullptr };
	const _tchar* m_pText = { nullptr };
	_vector m_vColor = { 1.f,1.f,1.f };
	_bool m_bShade = { false };
public:
	static CUIFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
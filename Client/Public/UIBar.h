#pragma once
#include "UIPanel.h"

BEGIN(Client)
class CUIBar :
    public CUIPanel
{
public:
	typedef struct UIBarDesc : public CUIPanel::PANEL_DESC
	{
		CTexture* pFillTextureCom = { nullptr };
		_float4 vFramePadding = { 0,0,0,0 };
		_float4 vFillBorder = { 0,0,0,0 };
	}UIBAR_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIBar";
protected:
	explicit CUIBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIBar(const CUIBar& Prototype);
	virtual ~CUIBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	void Update_Ratio(_float fRatio);
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render()override;

protected:
	CUIPanel* m_pFillPanel = { nullptr };
	_float m_fFillWidth = 0.f;
	_float m_fFillHeight = 0.f;
	_float m_fRatio = 0.f;
public:
	static CUIBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
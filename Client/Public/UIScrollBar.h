#pragma once
#include "UIPanel.h"

BEGIN(Client)
class CUIButton;
class CUIScrollBar :
    public CUIPanel
{
public:
	typedef struct ScrollBarDesc : public CUIPanel::PANEL_DESC
	{
		_float fScrollRatio = { 0.f };
		_float fScrollSpeed = { 0.1f };
	}SCROLLBAR_DESC;
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIScrollBar";

protected:
	CUIScrollBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIScrollBar(const CUIPanel& Prototype);
	virtual ~CUIScrollBar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;

	virtual void On_MouseLButtonDown(const POINT& tMousePoint)override;
	virtual void On_MouseLButtonUp() override;

	void Set_ScrollRatio(_float fRatio) { m_fScrollRatio = fRatio; }
	void Set_ScrollSpeed(_float fSpeed) { m_fScrollSpeed = fSpeed; }
private:
	CUIPanel* m_pScrollPanel = { nullptr };
	CUIButton* m_pScrollButton = { nullptr };
	CUIButton* m_pUpButton = { nullptr };
	CUIButton* m_pDownButton = { nullptr };
	_float m_fScrollRatio = { 0.f };
	_float m_fScrollSpeed = { 0.1f };
	_float m_fMinButtonY = { 10.f };
public:
	static CUIScrollBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once
#include "UIPanel.h"

BEGIN(Client)
class CUIScrollBar :
    public CUIPanel
{
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIScrollBar";

protected:
	CUIScrollBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIScrollBar(const CUIPanel& Prototype);
	virtual ~CUIScrollBar() = default;

private:
	CUIPanel* m_pScrollPanel = { nullptr };
	_float m_fScrollRatio = { 0.f };
	_float m_fScrollSpeed = { 0.1f };
public:
	static CUIScrollBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
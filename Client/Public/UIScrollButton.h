#pragma once
#include "UIButton.h"

BEGIN(Client)
class CUIScroller;
class CUIScrollButton :
	public CUIButton
{

public:
	typedef struct ScrollButtonDesc : public CUIButton::BUTTON_DESC
	{
		CUIScroller* pScroller = { nullptr };
		_float fMaxButtonHeight = { 100 };
		_float fMinButtonHeight = { 10 };
		_uint iTotalRowCount = { 1 };
		_uint iVisibleRowCount = { 1 };
		_float fMinOffsetY = { 0 };
		_float fMaxOffsetY = { 1 };
	}SCROLLBUTTON_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIScrollButton";

protected:
	explicit CUIScrollButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIScrollButton(const CUIScrollButton& rhs);
	virtual ~CUIScrollButton() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render()override;
	virtual void On_MouseDrag(const POINT& tMousePoint, const DIMOUSESTATE& tState) override;

	void Set_RowCounts(_uint iTotalRowCount, _uint iVisibleCount);
	void Set_Offset(_uint iRow);
private:
	CUIScroller* m_pScroller = { nullptr };
	_float m_fMaxButtonSize = { 100 };
	_float m_fMinButtonSize = { 10 };
	_float m_fMinOffsetY = { 0 };
	_float m_fMaxOffsetY = { 1 };

	_uint m_iTotalRowCount = { 1 };
	_uint m_iVisibleRowCount = { 1 };

public:
	static CUIScrollButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
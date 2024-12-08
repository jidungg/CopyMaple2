#pragma once
#include "UIContainer.h"


BEGIN(Client)
class CUIButton;
class CUIScrollButton;
class CUIList;
class CUIScroller :
    public CUIContainer
{
public:
	typedef struct ScrollBarDesc : public CUIObject::UIOBJECT_DESC
	{
		CUIList* pUIList = { nullptr };
		_uint iTotalRowCount = { 1 };
		_uint iVisibleRowCount = { 1 };
		_uint iInitialCursorPosition = { 0 };
		_float fScrollSpeed = { 1.f };
	}SCROLLBAR_DESC;
public:
    static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIScrollBar";

protected:
	CUIScroller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIScroller(const CUIScroller& Prototype);
	virtual ~CUIScroller() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;
	
	void Scroll_Up();
	void Scroll_Down();

	void Set_RowCounts(_uint iTotalRowCount, _uint iVisibleCount);
	void Set_CursorRow(_uint iRow);
private:
	CUIList* m_pUIList = { nullptr };
	CUIScrollButton* m_pScrollButton = { nullptr };
	CUIButton* m_pUpButton = { nullptr };
	CUIButton* m_pDownButton = { nullptr };

	_float m_fMinButtonY = { 10.f };
	_uint m_iTotalRowCount = { 1 };
	_uint m_iVisibleRowCount = { 1 };
	_uint m_iCursorPosition = { 0 };
public:
	static CUIScroller* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
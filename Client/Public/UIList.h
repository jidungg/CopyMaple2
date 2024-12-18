#pragma once
#include "UIContainer.h"
#include "UIListItemEntry.h"
#include "UIScroller.h"
#include "ObjectPool.h"

BEGIN(Engine)


END
BEGIN(Client)

class CUIList :
    public CUIContainer
{
public:
	typedef struct UIListDesc: public CUIObject::UIOBJECT_DESC
	{
		float fItemHeight = 70;
		float fItemWidth = 70;
		float fItemMarginX = 5;
		float fItemMarginY = 5;
		_uint iColumnCount = 10;
		_uint iRowCount = 10;
		_uint iObjectPoolSize = 200;

		LEVELID eItemEntryProtoLev;
		const _tchar* szItemEntryProtoTag;

		CUIScroller::SCROLLBAR_DESC tScrollerDesc = {};
	}UILIST_DESC;

protected:

	CUIList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIList(const CUIList& Prototype);
	virtual ~CUIList() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;



	void Set_YOffset(_float fYPos);
	//UIEntry의 크기는 늘어나지 않으므로, Row,Column을 먼저 세팅해야 함.
	HRESULT Set_ItemData(list<const ITEM_DATA*>* listData);
	HRESULT Set_ItemData(_uint iIdx, const ITEM_DATA* pData, _uint iItemCount = 1);
	HRESULT Resize(_uint iRow, _uint iCol);
	void Reposition();
	void Set_VisibleRowStart(_uint iRow) { m_iVisibleRowStart = iRow; }
	void Set_VisibleRowCount(_uint iCount) { m_iVisibleRowCount = iCount; }

	_bool Is_VisibleRow(_uint iRow);
	_uint Get_ItemRow(_uint iIndex);
	_float2 Get_ItemPos(_int iIndex);
	_uint Get_ItemCount() { return m_vecUIItem.size(); }
	_uint Get_ColCount() { return m_iColCount; }
	_uint Get_RowCount() { return m_iRowCount; }
	_float Get_HeightPerItem() { return m_fItemHeight + m_fItemMarginY; }
	_float Get_YOffset();
	_float Get_YMargin() { return m_fItemMarginY; }
private:
	float m_fItemHeight = 70;
	float m_fItemWidth = 70;
	float m_fItemMarginX = 5;
	float m_fItemMarginY = 5;
	_uint m_iColCount = 10;
	_uint m_iRowCount = 10;
	_uint m_iVisibleRowStart = 0;
	_uint m_iVisibleRowCount = 2;

	LEVELID m_eItemEntryProtoLev;
	const _tchar* m_szItemEntryProtoTag;
	CObjectPool< CUIObject>* m_pItemEntryPool = { nullptr };
protected:
	vector<IUIListItemEntry*> m_vecUIItem;
public:
	static CUIList* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		CUIList* pInstance = new CUIList(pDevice, pContext);

		if (FAILED(pInstance->Initialize_Prototype()))
		{
			MSG_BOX("Failed to Create : CUIList");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual CGameObject* Clone(void* pArg)
	{
		CUIList* pInstance = new CUIList(*this);

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Clone : CUIList");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual void Free() override
	{
		Safe_Release(m_pItemEntryPool);
		__super::Free();
	}
};


END




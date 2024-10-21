#pragma once
#include "UIPanel.h"
#include "UIListItemEntry.h"

BEGIN(Client)

class CUIList :
    public CUIPanel
{
public:
	typedef struct UIListDesc: public CUIPanel::PANEL_DESC
	{
		float fItemHeight = 70;
		float fItemWidth = 70;
		float fItemMarginX = 5;
		float fItemMarginY = 5;

		LEVELID eBackTexProtoLev;
		const _tchar* szBackTexProtoTag;
		list<UIListItemData*>* listData;
	}UILIST_DESC;

protected:

	CUIList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIList(const CUIList& Prototype);
	virtual ~CUIList() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	_float2 Get_ItemPos(_int iIndex);
private:
	float m_fItemHeight = 70;
	float m_fItemWidth = 70;
	float m_fItemMarginX = 5;
	float m_fItemMarginY = 5;
	
	LEVELID m_eBackTexProtoLev;
	const _tchar* m_szBackTexProtoTag;

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
		__super::Free();
	}
};


END




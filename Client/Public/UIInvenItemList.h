#pragma once
#include "UIListSelector.h"
BEGIN(Client)
class CInvenSlot;
class CUIInvenItemList :
    public CUIListSelector
{
protected:
	CUIInvenItemList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIInvenItemList(const CUIInvenItemList& Prototype);
	virtual ~CUIInvenItemList() = default;

public:
	HRESULT Set_ItemData(list<CInvenSlot*>* listData);
	HRESULT Set_ItemData(_uint iIndex, CInvenSlot* Data);
public:
	static CUIInvenItemList* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		CUIInvenItemList* pInstance = new CUIInvenItemList(pDevice, pContext);

		if (FAILED(pInstance->Initialize_Prototype()))
		{
			MSG_BOX("Failed to Create : CUIInvenItemList");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual CGameObject* Clone(void* pArg)
	{
		CUIInvenItemList* pInstance = new CUIInvenItemList(*this);

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Clone : CUIInvenItemList");
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
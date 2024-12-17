#pragma once
#include "UIList.h"
#include "UIButton.h"

BEGIN(Client)
class CUIListSelector :
    public CUIList
{
public:
	typedef struct UIListSelectorDesc : public CUIList::UILIST_DESC
	{
		_uint iInitialSelectedIndex = 0;
		LEVELID eHighlighterTexProtoLev;
		const _tchar* szHighlighterTexProtoTag;

	}UILISTSELECTOR_DESC;
protected:
	CUIListSelector(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIListSelector(const CUIListSelector& Prototype);
	virtual ~CUIListSelector() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;
	void Register_OnClickCallback(_uint iIdx, const ButtonCallback& fCallback);
	void Register_OnClickCallback(const ButtonCallback& fCallback);
	void Register_OnRightClickCallback(_uint iIdx, const ButtonCallback& fCallback);
	void Register_OnRightClickCallback(const ButtonCallback& fCallback);

	void Clear_OnClickCallback();
	void Clear_OnRightClickCallback();
	void On_ClickItem(void* pArg);
	void Select_Item(_uint iIndex);
	_uint Select_NextItem();

private:
	_uint m_iSelectedIndex = 0;
	CUIPanel* m_pHighlighter = nullptr;
public:
	static CUIListSelector* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	{
		CUIListSelector* pInstance = new CUIListSelector(pDevice, pContext);

		if (FAILED(pInstance->Initialize_Prototype()))
		{
			MSG_BOX("Failed to Create : CUIListSelector");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual CGameObject* Clone(void* pArg)
	{
		CUIListSelector* pInstance = new CUIListSelector(*this);

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Clone : CUIListSelector");
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
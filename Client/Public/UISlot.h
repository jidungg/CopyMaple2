#pragma once
#include "UIPanel.h"
BEGIN(Client)

class CUISlot :
    public CUIPanel
{
public:
	typedef struct UISlotDesc : public CUIPanel::PANEL_DESC
	{
		CUIPanel::PANEL_DESC tIconDesc = {};
	}UISLOT_DESC;
protected:
	explicit CUISlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUISlot(const CUISlot& Prototype);
	virtual ~CUISlot() = default;

public:
	HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;
	virtual bool Has_Item() abstract;
	virtual void Set_IconTexture(CTexture* pTexture);
	virtual void Compute_Matrix() override;
protected:
	CTexture* m_pIconTexure = nullptr;
	CRect_Transform* m_pIconTransform = nullptr;
public:
	virtual void Free() override;
};

END
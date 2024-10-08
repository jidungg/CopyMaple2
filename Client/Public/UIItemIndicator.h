#pragma once
#include "UIPanel.h"

BEGIN(Client)
class CUIItemIndicator :
    public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{

	}HOUSINGMATBUTTON_DESC;

protected:
	CUIItemIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIItemIndicator(const CUIItemIndicator& Prototype);
	virtual ~CUIItemIndicator() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
private:
	HRESULT Ready_Components();

protected:
	class CItem* m_pItem = nullptr;

public:
	static CUIItemIndicator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
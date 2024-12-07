#pragma once
#include "UIButton.h"

BEGIN(Client)
class CUIScrollButton :
	public CUIButton
{
protected:
	explicit CUIScrollButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIScrollButton(const CUIScrollButton& rhs);
	virtual ~CUIScrollButton() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;



private:

public:
	static CUIScrollButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END
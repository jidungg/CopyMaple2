#pragma once
#include "UIButton.h"
BEGIN(Client)
class CUIStatUpButton :
    public CUIButton
{
public:
	typedef struct UIStatUpButtonDesc : public CUIButton::ButtonDesc
	{
		_uint iStatType = { 0 };
	}UISTATUPBUTTON_DESC;

	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIStatUpButton";
protected:

	explicit CUIStatUpButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIStatUpButton(const CUIStatUpButton& Prototype);
	virtual ~CUIStatUpButton() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void On_MouseClick() override;

private:
	_uint m_iStatType = { 0 };
public:
	static CUIStatUpButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	void Free();
};


END
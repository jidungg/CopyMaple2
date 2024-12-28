#pragma once
#include "UIButton.h"
#include "Conversation.h"

BEGIN(Client)
class CUIIcon;
class CUIFont;
class CUIChatOption :
    public CUIButton
{
public:
	typedef struct ChatOptionDesc : public CUIButton::BUTTON_DESC
	{

	}CHATOPTION_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIChatOption";
protected:
	CUIChatOption(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIChatOption(const CUIChatOption& Prototype);
	virtual ~CUIChatOption() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;

	void Set_Option(_uint iDataOptIndex, const ChatOptionData& tOptionData);
	_uint Get_DataOptionIdx() { return m_iDataOptionIdx; }
	const ChatOptionData& Get_OptionData() { return m_tOptionData; }
private:
	_uint m_iDataOptionIdx = 0;
	CUIIcon* m_pIcon = { nullptr };
	CUIFont* m_pFont = { nullptr };
	ChatOptionData m_tOptionData;
public:
	static CUIChatOption* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
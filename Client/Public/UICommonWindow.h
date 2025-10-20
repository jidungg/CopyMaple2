#pragma once
#include "UIPanel.h"

BEGIN(Engine)
class CTexture;
END
BEGIN(Client)
//기능 : 왼쪽 위 아이콘 & 창 이름, 오른쪽 위 닫기 버튼
class CUIFont;
class CUICommonWindow :
    public CUIPanel
{
public:
	typedef struct UICommonWindowDesc : public CUIPanel::PANEL_DESC
	{
		const _tchar* szIconProtoTag = { nullptr };
		const _tchar* szTitleText = { nullptr };
	}UICOMMONWINDOW_DESC;

protected:
	explicit CUICommonWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUICommonWindow(const CUICommonWindow& Prototype);
	virtual ~CUICommonWindow() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render()override;

	void Close_Window();
protected:
	_float m_fCommonMargin = { 5.f };
	_float m_fCommonButtonSize = { 24.f };
public:
	virtual void Free() override;

};

END
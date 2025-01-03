#pragma once
#include "UIPanel.h"
#include "Status.h"

BEGIN(Client)
class CUIEXPBar :
    public CUIPanel
{
public:
	typedef struct UIEXPBarDesc : public CUIPanel::PanelDesc
	{
		Stat* pStat = { nullptr };
		Stat* pDefaultStat = { nullptr };
	}UIEXPBAR_DESC;

	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIEXPBar";
protected:
	explicit CUIEXPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIEXPBar(const CUIEXPBar& Prototype);
	virtual ~CUIEXPBar() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;

private:
	Stat* m_pStat = { nullptr };
	Stat* m_pDefaultStat = { nullptr };
	CUIVerticalFill<_int>* m_pFill = { nullptr };

public:
	static CUIEXPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	void Free();
};

END
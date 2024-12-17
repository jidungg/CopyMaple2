#pragma once
#include "UIPanel.h"
BEGIN(Client)
class CUIIcon :
    public CUIPanel
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIIcon";
private:
	explicit CUIIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIIcon(const CUIIcon& Prototype);
	virtual ~CUIIcon() = default;

public:
    virtual bool Check_MouseOver(POINT fPos);

public:
	static CUIIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
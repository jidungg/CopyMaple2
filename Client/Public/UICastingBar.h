#pragma once
#include "UIBar.h"
BEGIN(Client)
class CUICastingBar :
    public CUIBar
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUICastingBars";
protected:
	explicit CUICastingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUICastingBar(const CUIBar& Prototype);
	virtual ~CUICastingBar() = default;

public:
	virtual void Late_Update(_float fTimeDelta) override;

public:
	static CUICastingBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#pragma once
#include "UIBar.h"

BEGIN(Client)
class CCharacter;
class CUIHPBar :
    public CUIBar
{
public:
	typedef struct UIHPBarDesc : public CUIBar::UIBAR_DESC
	{
		CCharacter* pCharacter = nullptr;
	}HPBAR_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIHPBar";
protected:
	explicit CUIHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIHPBar(const CUIHPBar& Prototype);
	virtual ~CUIHPBar() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual void Set_Character(CCharacter* pCharacter) {m_pCharacter = pCharacter;}
private:
	CCharacter* m_pCharacter = nullptr;

public:
	static CUIHPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
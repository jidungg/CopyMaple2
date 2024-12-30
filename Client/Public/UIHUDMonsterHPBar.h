#pragma once
#include "UIHPBar.h"

BEGIN(Client)
class CUIFont;
class CMonster;
class CUIHUDMonsterHPBar :
    public CUIHPBar
{
public:
	typedef struct HUDMonsterHPBarDesc : public CUIBar::UIBAR_DESC
	{
		_bool bBoss = { false };
	}HUDMONSTERHPBAR_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIHUDMonsterHPBar";
protected:
	explicit CUIHUDMonsterHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIHUDMonsterHPBar(const CUIHUDMonsterHPBar& Prototype);
	virtual ~CUIHUDMonsterHPBar() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual void Set_Character(CCharacter* pCharacter)override;
private:
	_bool m_bBoss = { false };
	CUIFont* m_pRemainHPFont = { nullptr };
	CUIFont* m_pNameFont = { nullptr };
	CUIFont* m_pLevelFont = { nullptr };
	_float2 m_f2LevelFontPos = { 20,26 };
public:
	static CUIHUDMonsterHPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
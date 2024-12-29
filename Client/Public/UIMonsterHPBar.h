#pragma once
#include "UIBar.h"

BEGIN(Client)
class CMonster;
class CUIHUDMonsterHPBar :
    public CUIBar
{
public:
	typedef struct UIMonsterHPBarDesc : public CUIBar::UIBAR_DESC
	{
	}MONSTERHPBAR_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIBossHPBar";
protected:
	explicit CUIHUDMonsterHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIHUDMonsterHPBar(const CUIHUDMonsterHPBar& Prototype);
	virtual ~CUIHUDMonsterHPBar() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render()override;
	void Set_Monster(CMonster* pMonster);
private:
	CMonster* m_pMonster = nullptr;
public:
	static CUIHUDMonsterHPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#pragma once
#include "WorldUIObject.h"
#include "UIHPBar.h"

BEGIN(Client)
class CCharacter;
class CWorldUIHPBar :
    public CWorldUIObject
{
public:
	typedef struct UIWorldHPBarDesc : public CWorldUIObject::WorldUIObjDesc
	{
		CCharacter* pCharacter = { nullptr };
	}WORLDHPBAR_DESC;
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIWorldHPBar";

protected:
	explicit CWorldUIHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CWorldUIHPBar(const CWorldUIHPBar& Prototype);
	virtual ~CWorldUIHPBar() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;

private:
	CCharacter* m_pCharacter = { nullptr };
	CUIHPBar* m_pHPBar = { nullptr };
public:
	static CWorldUIHPBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
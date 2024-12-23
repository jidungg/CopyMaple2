#pragma once
#include "UIContainer.h"
#include "Status.h"

BEGIN(Client)
class CUIPanel;
template<typename T>
class CUIVerticalFill;
class CUIFont;
class CUIMainHUDGuage :
    public CUIContainer
{
public:
	typedef struct UIMainHUDGuageDesc : public CUIContainer::UIOBJECT_DESC
	{
		Stat* pStat = { nullptr };
		Stat* pDefaultStat = { nullptr };
	}UIMAINHUDGUAGE_DESC;

	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIMainHPBarDesc";
protected:
	explicit CUIMainHUDGuage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIMainHUDGuage(const CUIMainHUDGuage& Prototype);
	virtual ~CUIMainHUDGuage() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;
private:
	Stat* m_pStat = { nullptr };
	Stat* m_pDefaultStat = { nullptr };
	CUIPanel* m_pBack = { nullptr };
	CUIPanel* m_pFront = { nullptr };
	CUIVerticalFill<_int>* m_pRedFill = { nullptr };
	CUIVerticalFill<_int>* m_pBlueFill = { nullptr };
	CUIPanel* m_pWhiteFill[3] = {nullptr};

	CUIFont* m_pHPFont = { nullptr };
	CUIFont* m_pSPFont = { nullptr };
	CUIFont* m_pHPCountFont = { nullptr };
	CUIFont* m_pSPCountFont = { nullptr };
	CUIFont* m_pDefaultHPCountFont = { nullptr };
	CUIFont* m_pDefaultSPCountFont = { nullptr };

public:
	static CUIMainHUDGuage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	void Free();
};

END
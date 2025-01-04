#pragma once
#include "UIContainer.h"
#include "Status.h"

BEGIN(Client)
class CUIPanel;
class CPlayerInfo;
template<typename T>
class CUIVerticalFill;
class CUIFont;
class CUIMainHUDGuage :
    public CUIContainer
{
public:
	typedef struct UIMainHUDGuageDesc : public CUIContainer::UIOBJECT_DESC
	{

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

	void Set_HP(_int iHP);
	void Set_SP(_int iSP);
	void Set_DefaultHP(_int iHP);
	void Set_DefaultSP(_int iSP);
	void Set_EP(_int iEP);
private:
	_int m_iHP = { 0 };
	_int m_iDefaultHP = { 0 };
	_int m_iSP = { 0 };
	_int m_iDefaultSP = { 0 };
	_int m_iEP = { 0 };

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

	CPlayerInfo* m_pPlayerInfo = { nullptr };
public:
	static CUIMainHUDGuage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	void Free();
};

END
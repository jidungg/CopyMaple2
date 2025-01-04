#pragma once
#include "UIPanel.h"

BEGIN(Client)
class CPlayer;
class CUIFont;
class CUIButton;
class CUIStatUpButton;
class CPlayerInfo;
class CUIPlayerInfoDashBoard :
    public CUIPanel
{
public:
	typedef struct PlayerInfoDashBoardDesc : public CUIPanel::PANEL_DESC
	{
		CPlayerInfo* pPlayerInfo = { nullptr };
	}PLAYERINFODASHBOARD_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIPlayerInfoDashBoard";
public:
	explicit CUIPlayerInfoDashBoard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIPlayerInfoDashBoard(const CUIPlayerInfoDashBoard& Prototype);
	virtual ~CUIPlayerInfoDashBoard() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

private:
	CPlayerInfo* m_pPlayerInfo = { nullptr };

	CUIFont* m_pBattleTextFont = { nullptr };
	CUIFont* m_pAttackTextFont = { nullptr };
	CUIFont* m_pDefenseTextFont = { nullptr };
	CUIFont* m_pSTRTextFont = { nullptr };
	CUIFont* m_pDEXTextFont = { nullptr };
	CUIFont* m_pINTTextFont = { nullptr };
	CUIFont* m_pLUKTextFont = { nullptr };
	CUIFont* m_pHPTextFont = { nullptr };
	CUIFont* m_pCRITTextFont = { nullptr };
	CUIFont* m_pLeftStatTextFont = { nullptr };

	CUIFont* m_pBattlePointFont = { nullptr };
	CUIFont* m_pAttackPointFont = { nullptr };
	CUIFont* m_pDefensePointFont = { nullptr };
	CUIFont* m_pSTRPointFont = { nullptr };
	CUIFont* m_pDEXPointFont = { nullptr };
	CUIFont* m_pINTPointFont = { nullptr };
	CUIFont* m_pLUKPointFont = { nullptr };
	CUIFont* m_pHPPointFont = { nullptr };
	CUIFont* m_pCRITPointFont = { nullptr };
	CUIFont* m_pLeftStatPointFont = { nullptr };

	CUIStatUpButton* m_pSTRUpButton = { nullptr };
	CUIStatUpButton* m_pDEXUpButton = { nullptr };
	CUIStatUpButton* m_pINTUpButton = { nullptr };
	CUIStatUpButton* m_pLUKUpButton = { nullptr };
	CUIStatUpButton* m_pHPUpButton = { nullptr };
	CUIStatUpButton* m_pCRITUpButton = { nullptr };

	_float2 m_fBigCellSize = {232,56};
	_float2 m_fMediumCellSize = {232,42};
	_float2 m_fSmallCellSize = {232,30};
	_float2 m_fUpButtonSize = { 26,26 };
	_float m_fPadding = { 3.f };
	_float m_fMargin = { 3.f };

public:
	static CUIPlayerInfoDashBoard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
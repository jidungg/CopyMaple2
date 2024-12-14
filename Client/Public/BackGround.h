#pragma once

#include "Client_Defines.h"
#include "UIPanel.h"
#include "GameObject.h"


BEGIN(Client)
class CPlayer;
class CCubeTerrain;
class CBackGround final : public CUIPanel
{
public:
	typedef struct BackgroundDesc: public CUIPanel::PANEL_DESC
	{
		CPlayer* pPlayer = { nullptr };
		CCubeTerrain* pTerrain = { nullptr };
		const _tchar* szBackGroundImgTag = { nullptr };
		LEVELID eBackGroundImgProtoLevel = { LEVELID::LAST };
	}BACKGROUND_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Background";

private:
	CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackGround(const CBackGround& Prototype);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CPlayer* m_pPlayer = { nullptr };
	_float m_fSizeX;
	_float m_fSizeY;
	_float m_fLeftEdge;
	_float m_fRightEdge;
	_float m_fTopEdge;
	_float m_fBottomEdge;
	_vector m_vHorizontalPlane;
	_vector m_vVerticalPlane;
public:
	static CBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};


END
#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CUIQuickSlot;
class CPlayer;
class CModelObject;
class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;	
private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);

private:
	CUIQuickSlot* m_pQuickSlot = { nullptr };
	CPlayer* m_pPlayer = { nullptr };
	CModelObject* m_pModel = { nullptr };
	CModelObject* m_pHair = nullptr;
public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "ItemDataBase.h"


BEGIN(Client)
class CBuilder;
class CCubeTerrain;
class CCamera_Trace;
class CPlayer;
class CUIHomeDialog;
class CLevel_Home final:
    public CLevel
{
private:
	explicit CLevel_Home(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Home() = default;

public:
	virtual HRESULT Initialize()override;
	virtual void Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	void On_BuildItemSelected(void*);
	void Set_BuildItem(BUILD_ITEM_ID eID);
private:
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Lights();


private:
	CCubeTerrain* m_pCubeTerrain = { nullptr };
	CBuilder* m_pBuilder = { nullptr };
	CCamera_Trace* m_pCamera = { nullptr };
	CPlayer* m_pPlayer = { nullptr };

	map<_uint,  ITEM_DESC*>* m_pItemData = nullptr;
	map<_uint,  ITEM_DESC*>::iterator m_pItemIter = {};
	bool m_bBuildMode = false;

	CUIHomeDialog* m_pHomeDialog = { nullptr };
public:
	static CLevel_Home* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

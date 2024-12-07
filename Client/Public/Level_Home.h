#pragma once
#include "Client_Defines.h"
#include "Level_GamePlay.h"
#include "ItemDataBase.h"


BEGIN(Client)
class CBuilder;
class CCubeTerrain;
class CCamera_Trace;
class CPlayer;
class CUIHomeDialog;
class CLevel_Home final:
	public CLevel_GamePlay
{
private:
	explicit CLevel_Home(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Home() = default;

public:
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	virtual void On_Start(_uint iPrevLevelID)override ;


	void On_BuildItemSelected(void*);
	void Set_BuildItem(_uint iID);
private:
	HRESULT Ready_Layer_UI(LAYERID eLayerId);

private:

	CBuilder* m_pBuilder = { nullptr };
	CCamera_Trace* m_pCamera = { nullptr };

	map<_uint,  ITEM_DATA*>* m_pItemData = nullptr;
	map<_uint,  ITEM_DATA*>::iterator m_pItemIter = {};
	bool m_bBuildMode = false;

	CUIHomeDialog* m_pHomeDialog = { nullptr };
public:
	static CLevel_Home* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual void Free() override;
};

END

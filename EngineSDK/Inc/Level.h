#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void On_Start(_uint iPrevLevel);
	virtual void On_End(_uint iNextLevel);
	_int Get_LevelID() { return m_iLevelID; }
protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	_int						m_iLevelID = {-1};
public:
	virtual void Free() override;
};

END
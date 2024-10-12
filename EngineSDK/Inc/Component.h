#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& Prototype);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) {};
	virtual void Update(_float fTimeDelta) {};
	virtual void Late_Update(_float fTimeDelta) {};

	bool Is_Active() { return m_bActive; }
	void Set_Active(bool bValue) { m_bActive = bValue; }
	void Set_Owner(class CGameObject* pOwner) { m_pOwner = pOwner; }
protected:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };
	class CGameInstance*			m_pGameInstance = { nullptr };
	class CGameObject*				m_pOwner = { nullptr };
	bool m_bActive = true;
public:	
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
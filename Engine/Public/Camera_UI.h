#pragma once
#include "Camera.h"

BEGIN(Engine)
class ENGINE_DLL CCamera_UI :
    public CCamera
{
private:
	CCamera_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_UI(const CCamera_UI& Prototype);
	virtual ~CCamera_UI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CCamera_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
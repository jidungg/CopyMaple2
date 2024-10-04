#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)
class CCamera_Trace :
    public CCamera
{
public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		_float3			vArm = {};
	}TRACECAMERA_DESC;
private:
	explicit CCamera_Trace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCamera_Trace(const CCamera_Trace& Prototype);
	virtual ~CCamera_Trace() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

private:
	_float3			m_vArm = {};
public:
	static CCamera_Trace* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
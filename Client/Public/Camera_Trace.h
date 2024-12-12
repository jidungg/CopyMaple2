#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)
class CCamera_Trace :
    public CCamera
{
public:
	typedef struct TraceCamDesc: public CCamera::CAMERA_DESC
	{
		_vector			vFarArm = {};
		_vector			vCloseArm = {};
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
	void Set_Ratio(_float fRatio) { m_fCurrentRatio = fRatio; }
private:
	_vector			m_vFarArm = {};
	_vector			m_vCloseArm = {};
	//0이면 Far, 1이면 Close
	_float				m_fCurrentRatio = { 0 };
	
public:
	static CCamera_Trace* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
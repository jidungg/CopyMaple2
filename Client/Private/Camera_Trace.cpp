#include "stdafx.h"
#include "Camera_Trace.h"
#include "GameInstance.h"

CCamera_Trace::CCamera_Trace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Trace::CCamera_Trace(const CCamera_Trace& Prototype)
	: CCamera(Prototype)
{
}

HRESULT CCamera_Trace::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Trace::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	TRACECAMERA_DESC* pDesc = static_cast<TRACECAMERA_DESC*>(pArg);
	m_vFarArm = pDesc->vFarArm * 10.f;
	m_vCloseArm = pDesc->vCloseArm;
	m_fCurrentRatio = 0;
	return S_OK;
}

void CCamera_Trace::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CCamera_Trace::Update(_float fTimeDelta)
{
	_float iMove = m_pGameInstance->Get_DIMouseMove(MOUSE_MOVE::Z);
	m_fCurrentRatio += (iMove) * fTimeDelta * 0.05;
	m_fCurrentRatio = clamp(m_fCurrentRatio,0.f, 1.f);

	__super::Update(fTimeDelta);
}

void CCamera_Trace::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	_vector vAt = Get_Target()->Get_WorldPosition();
	
	_vector vEye = vAt + XMVectorLerp(m_vFarArm, m_vCloseArm, m_fCurrentRatio);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vEye, 1.f));
	m_pTransformCom->LookAt(XMVectorSetW(vAt,1));
	__super::Compute_PipeLineMatrices();

}



CCamera_Trace* CCamera_Trace::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Trace* pInstance = new CCamera_Trace(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CCamera_Trace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Trace::Clone(void* pArg)
{
	CCamera_Trace* pInstance = new CCamera_Trace(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCamera_Trace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Trace::Free()
{
	__super::Free();
}

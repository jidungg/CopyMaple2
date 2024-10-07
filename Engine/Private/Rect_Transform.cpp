#include "Rect_Transform.h"

CRect_Transform::CRect_Transform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTransform(pDevice, pContext)
{
}

CRect_Transform::CRect_Transform(const CRect_Transform& Prototype)
	: CTransform(Prototype),
	m_fSizeX{ Prototype.m_fSizeX },
	m_fSizeY{ Prototype.m_fSizeY },
	m_fXOffset{ Prototype.m_fXOffset },
	m_fYOffset{ Prototype.m_fYOffset },
	m_eAnchorType{ Prototype.m_eAnchorType },
	m_ePivotType{ Prototype.m_ePivotType }
{
}

HRESULT CRect_Transform::Initialize_Prototype()
{
	return __super::Initialize_Prototype();
}

HRESULT CRect_Transform::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	RECTTRANSFORM_DESC* pDesc = static_cast<RECTTRANSFORM_DESC*>(pArg);

	m_eAnchorType = pDesc->eAnchorType;
	m_ePivotType = pDesc->ePivotType;
	m_fXOffset = pDesc->fXOffset;
	m_fYOffset = pDesc->fYOffset;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;
	m_fXPosition = 0;
	m_fYPosition = 0;


	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	m_iViewportWidth = (ViewportDesc.Width);
	m_iViewportHeight = (ViewportDesc.Height);

	Compute_Matrix();
	return S_OK;
}

void CRect_Transform::Priority_Update(_float fTimeDelta)
{
}

void CRect_Transform::Update(_float fTimeDelta)
{

}

void CRect_Transform::Late_Update(_float fTimeDelta)
{
}

void CRect_Transform::Compute_Matrix()
{
	_float2 fParentAnchor = Get_AnchorPosition(m_eAnchorType);
	_float2 fPivotOffset = Get_PivotOffset(m_ePivotType);
	m_fXPosition = fParentAnchor.x + m_fXOffset - fPivotOffset.x;
	m_fYPosition = fParentAnchor.y + m_fYOffset - fPivotOffset.y;

	Scaling(m_fSizeX, m_fSizeY, 1.f);
	Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fXPosition - m_iViewportWidth * 0.5, -m_fYPosition + m_iViewportHeight * 0.5, 0.f, 1.f));

}

void CRect_Transform::Go_Straight(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = XMVectorSet(0,1,0,0);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CRect_Transform::Go_Backward(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = XMVectorSet(0, -1, 0, 0);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CRect_Transform::Go_Left(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = XMVectorSet(-1, 0, 0, 0);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CRect_Transform::Go_Right(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = XMVectorSet(1, 0, 0, 0);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CRect_Transform::Move(_float fX, _float fY)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = XMVectorSet(fX, fY, 0, 0);
	vPosition += XMVector3Normalize(vLook);
	Set_State(STATE_POSITION, vPosition);
}


void CRect_Transform::Set_Offset(_float fX, _float fY)
{
	m_fXOffset = fX;
	m_fYOffset = fY;
	Compute_Matrix();
}

void CRect_Transform::Set_Size(_float fSizeX, _float fSizeY)
{
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
	Compute_Matrix();
}

_float2 CRect_Transform::Get_PivotOffset(CORNOR_TYPE ePivotType)
{
	_float2		fPivotOffset{ 0,0 };
	_float2		fAdditional{ 0,0 };
	fAdditional = Get_CornorRatio(ePivotType);

	fPivotOffset.x = m_fSizeX * fAdditional.x;
	fPivotOffset.y = m_fSizeY * fAdditional.y;
	return fPivotOffset;
}

_float2 CRect_Transform::Get_PivotPosition(CORNOR_TYPE ePivotType)
{
	_float2		fPivotPoint = Get_PivotOffset(ePivotType);
	fPivotPoint.x += m_fXPosition;
	fPivotPoint.y += m_fYPosition;
	return fPivotPoint;
}

_float2 CRect_Transform::Get_AnchorPosition(CORNOR_TYPE eAnchorType)
{
	_float2		fAnchorPoint;
	CRect_Transform* pParent = dynamic_cast<CRect_Transform*>(m_pParentTransform);

	if (pParent == nullptr)
	{
		_float2 fRatio = Get_CornorRatio(eAnchorType);
		fAnchorPoint = _float2(m_iViewportWidth * 0.5 + m_iViewportWidth * fRatio.x, m_iViewportHeight * 0.5 + m_iViewportHeight * fRatio.y);
	}
	else
	{
		fAnchorPoint = pParent->Get_PivotPosition(eAnchorType);
	}

	return fAnchorPoint;
}

bool CRect_Transform::Is_InRect(_float2 vPosition)
{
	return  (vPosition.x <= m_fXPosition + m_fSizeX * 0.5f &&
		vPosition.x >= m_fXPosition - m_fSizeX * 0.5f &&
		vPosition.y <= m_fYPosition + m_fSizeY * 0.5f &&
		vPosition.y >= m_fYPosition - m_fSizeY * 0.5f);
}

_float2 CRect_Transform::Get_CornorRatio(CORNOR_TYPE ePivotType)
{
	_float2		fPivotOffset{};

	switch (ePivotType)
	{
	case Engine::CORNOR_TYPE::LEFT_TOP:
		fPivotOffset = _float2(-0.5f, -0.5f);
		break;
	case Engine::CORNOR_TYPE::TOP:
		fPivotOffset = _float2(0.f, -0.5f);
		break;
	case Engine::CORNOR_TYPE::RIGHT_TOP:
		fPivotOffset = _float2(0.5f, -0.5f);
		break;
	case Engine::CORNOR_TYPE::LEFT:
		fPivotOffset = _float2(-0.5f, 0.f);
		break;
	case Engine::CORNOR_TYPE::CENTER:
		fPivotOffset = _float2(0.f, 0.f);
		break;
	case Engine::CORNOR_TYPE::RIGHT:
		fPivotOffset = _float2(0.5f, 0.0f);
		break;
	case Engine::CORNOR_TYPE::LEFT_BOT:
		fPivotOffset = _float2(-0.5f, 0.5f);
		break;
	case Engine::CORNOR_TYPE::BOT:
		fPivotOffset = _float2(0.f, 0.5f);
		break;
	case Engine::CORNOR_TYPE::RIGHT_BOT:
		fPivotOffset = _float2(0.5f, 0.5f);
		break;
	case Engine::CORNOR_TYPE::LAST:
		fPivotOffset = _float2(-1.f, -1.f);
		break;
	default:
		break;
	}
	return fPivotOffset;
}

CTransform* CRect_Transform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRect_Transform* pInstance = new CRect_Transform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRect_Transform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRect_Transform::Clone(void* pArg)
{
	CRect_Transform* pInstance = new CRect_Transform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRect_Transform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRect_Transform::Free()
{
	__super::Free();
}

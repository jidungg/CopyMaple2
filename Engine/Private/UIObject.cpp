#include "..\Public\UIObject.h"

#include "GameInstance.h"

CUIObject::CUIObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject & Prototype)
	: CGameObject{ Prototype },
	m_fSizeX{ Prototype.m_fSizeX },
	m_fSizeY{ Prototype.m_fSizeY },
	m_fXOffset{ Prototype.m_fXOffset },
	m_fYOffset{ Prototype.m_fYOffset },
	m_eAnchorType{ Prototype.m_eAnchorType },
	m_ePivotType{ Prototype.m_ePivotType }	
{
}

HRESULT CUIObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUIObject::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	
	UIOBJECT_DESC*	pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;
	m_fXOffset = pDesc->fXOffset;
	m_fYOffset = pDesc->fYOffset;
	m_eAnchorType = pDesc->eAnchorType;
	m_ePivotType = pDesc->ePivotType;

	m_fXPosition = 0;
	m_fYPosition = 0;

	_uint		iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	m_iViewportWidth = (ViewportDesc.Width);
	m_iViewportHeight = (ViewportDesc.Height);

	/* 직교튀영을 위한 뷰ㅡ, 투영행르을 만들었다. */
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_iViewportWidth, m_iViewportHeight, 0.f, 1.f));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* 던져준 fX, fY,  fSizeX, fSizeY로 그려질 수 있도록 월드행렬의 상태를 제어해둔다. */
	Update(0);

	return S_OK;
}

//부모의 Position과 자신의 Pivot을 이용하여 자신의 위치를 계산한다.
void CUIObject::Update(_float fTimeDelta)
{
	if (false == m_bActive) return;

	_float2 fParentAnchor = Get_AnchorPoint(m_eAnchorType);
	_float2 fPivot;
	fPivot.x = fParentAnchor.x + m_fXOffset;
	fPivot.y = fParentAnchor.y + m_fYOffset;
	_float2 fPivotOffset = Get_PivotOffset(m_ePivotType);
	m_fXPosition = fPivot.x - fPivotOffset.x;
	m_fYPosition = fPivot.y - fPivotOffset.y;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fXPosition - m_iViewportWidth*0.5, -m_fYPosition + m_iViewportHeight * 0.5, 0.f, 1.f));

	for (auto& child: m_pChildUIs)
	{
		child->Update(fTimeDelta);
	}
}

void CUIObject::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive) return;

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}


HRESULT CUIObject::Render()
{
	Render_Self();
	for (auto& child : m_pChildUIs)
		child->Render_Self();

	return S_OK;
}

void CUIObject::Add_ChildUI(CUIObject* pChildUI)
{
	if (pChildUI == nullptr)
		return;
	m_pChildUIs.push_back(pChildUI);
	pChildUI->m_pParentUI = this;
	Safe_AddRef(pChildUI);
	
}

void CUIObject::On_MouseEnter(const DIMOUSESTATE& tMouseState)
{
}

void CUIObject::On_MouseOver(const DIMOUSESTATE& tMouseState)
{
}

void CUIObject::On_MouseExit(const DIMOUSESTATE& tMouseState)
{
}

bool CUIObject::Check_MouseOver(LONG lX, LONG lY)
{

	return false;
}

void CUIObject::Set_Offset(_float fX, _float fY)
{
	m_fXOffset = fX;
	m_fYOffset = fY;
}

void CUIObject::Set_Size(_float fSizeX, _float fSizeY)
{
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
}

_float2 CUIObject::Get_CornorRatio(CORNOR_TYPE ePivotType)
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
//Pivot의 자신 중앙 기준 Offset
_float2 CUIObject::Get_PivotOffset(CORNOR_TYPE ePivotType)
{
	_float2		fPivotOffset{0,0};
	_float2		fAdditional { 0,0 };
	fAdditional = Get_CornorRatio(ePivotType);

	fPivotOffset.x = m_fSizeX * fAdditional.x;
	fPivotOffset.y = m_fSizeY * fAdditional.y;
	return fPivotOffset;
}
//Pivot의 Screen 좌표 = 자신의 Position  + PivotOffset
_float2 CUIObject::Get_PivotPoint(CORNOR_TYPE ePivotType)
{
	_float2		fPivotPoint = Get_PivotOffset(ePivotType);
	fPivotPoint.x += m_fXPosition;
	fPivotPoint.y += m_fYPosition;
	
	return fPivotPoint;
}
//Anchor의 Screen 기준 좌표 = 부모의 코너의 Position
_float2 CUIObject::Get_AnchorPoint(CORNOR_TYPE eAnchorType)
{
	_float2		fAnchorPoint;
	if (m_pParentUI == nullptr)
	{
		_float2 fRatio = Get_CornorRatio(eAnchorType);
		fAnchorPoint = _float2(m_iViewportWidth*0.5 + m_iViewportWidth * fRatio.x, m_iViewportHeight*0.5 + m_iViewportHeight * fRatio.y);
	}
	else
	{
		fAnchorPoint = m_pParentUI->Get_PivotPoint(eAnchorType);
	}

	return fAnchorPoint;
}



void CUIObject::Free()
{
	__super::Free();
	for (auto& i : m_pChildUIs)
	{
		Safe_Release(i);
	}
	m_pChildUIs.clear();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

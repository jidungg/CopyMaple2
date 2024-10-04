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
	m_fYOffset{ Prototype.m_fYOffset }
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
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fXPosition - m_iViewportWidth * 0.5f, -m_fYPosition + m_iViewportHeight * 0.5f, 0.f, 1.f));


	return S_OK;
}

void CUIObject::Update(_float fTimeDelta)
{
	if (false == m_bActive) return;

	if (m_pParentUI == nullptr)
	{
		m_fXPosition = m_fXOffset;
		m_fYPosition = m_fYOffset;
	}
	else
	{
		m_fXPosition = m_pParentUI->m_fXPosition + m_fXOffset;
		m_fYPosition = m_pParentUI->m_fYPosition + m_fYOffset;
	}

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fXPosition - m_iViewportWidth * 0.5f, -m_fYPosition + m_iViewportHeight * 0.5f, 0.f, 1.f));

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



void CUIObject::Free()
{
	__super::Free();
	for (auto& i : m_pChildUIs)
	{
		Safe_Release(i);
	}
	m_pChildUIs.clear();
}

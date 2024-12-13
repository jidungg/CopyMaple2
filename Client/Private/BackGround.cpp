#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"
#include "Player.h"
#include "CubeTerrain.h"

CBackGround::CBackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIPanel{ pDevice, pContext }
{
}

CBackGround::CBackGround(const CBackGround & Prototype)
	: CUIPanel{ Prototype }
{
}


HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{
	BACKGROUND_DESC* pDesc = static_cast<BACKGROUND_DESC*>(pArg);
	m_pPlayer =pDesc->pPlayer;
	pDesc->eAnchorType = CORNOR_TYPE::CENTER;
	pDesc->ePivotType = CORNOR_TYPE::CENTER;
	pDesc->fXOffset = 0;
	pDesc->fYOffset = 0;
	m_fSizeX = pDesc->fSizeX = g_iWinSizeX<<1;
	m_fSizeY = pDesc->fSizeY = g_iWinSizeY<<1;
	pDesc->pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(pDesc->szBackGroundImgTag, nullptr));
	pDesc->bRegisterUIManager = false;

	m_vHorizontalPlane = XMPlaneFromPointNormal({ 0,0,0 }, XMVector3Normalize( { 1,0,-1 }));
	XMUINT3 vMapSize = pDesc->pTerrain->Get_Size();
	_float fVerticalDist = pDesc->pTerrain->Get_Distance(0, vMapSize.x * vMapSize.z-1);
	m_vVerticalPlane = XMPlaneFromPointNormal({ 0,fVerticalDist/2,0 }, XMVector3Normalize({ 1,0,1 }));

	m_fLeftEdge = (_float)g_iWinSizeX / 2.f - m_fSizeX / 2.f;
	m_fRightEdge = (_float)m_fSizeX / 2.f - (_float)g_iWinSizeX /2.f;
	m_fTopEdge = m_fSizeY / 2.f - (_float)g_iWinSizeY / 2.f;
	m_fBottomEdge = (_float)g_iWinSizeY / 2.f - m_fSizeY / 2.f;
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	_vector vPos = Get_WorldPosition();
	_vector vPlayerPos = m_pPlayer->Get_WorldPosition();
	vPos.m128_f32[0] = -XMPlaneDotCoord( m_vHorizontalPlane ,vPlayerPos).m128_f32[0]* 30;
	vPos.m128_f32[1] = -XMPlaneDotCoord(m_vVerticalPlane, vPlayerPos).m128_f32[0] * 30;
	vPos.m128_f32[0] = clamp(vPos.m128_f32[0], m_fLeftEdge  , m_fRightEdge );
	vPos.m128_f32[1] = clamp(vPos.m128_f32[1], m_fBottomEdge , m_fTopEdge);
m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY,this);
}

HRESULT CBackGround::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_pShaderCom)
		m_pShaderCom->Begin(2);
	if (m_pVIBufferCom)
	{
		m_pVIBufferCom->Bind_BufferDesc();
		m_pVIBufferCom->Render();
	}

	return S_OK;
}


CBackGround * CBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBackGround*	pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround*	pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

}

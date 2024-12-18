#include "stdafx.h"
#include "UIScrollButton.h"
#include "GameInstance.h"
#include "UIScroller.h"

CUIScrollButton::CUIScrollButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIButton(pDevice, pContext)
{
}

CUIScrollButton::CUIScrollButton(const CUIScrollButton& rhs)
	: CUIButton(rhs)
{
}



HRESULT CUIScrollButton::Initialize(void* pArg)
{
	SCROLLBUTTON_DESC* pDesc = static_cast<SCROLLBUTTON_DESC*>(pArg);
	m_pScroller = pDesc->pScroller;
	m_fMaxButtonSize = pDesc->fMaxButtonHeight;
	m_fMinButtonSize = pDesc->fMinButtonHeight;
	m_fMinOffsetY = pDesc->fMinOffsetY;
	m_fMaxOffsetY = pDesc->fMaxOffsetY;
	m_iTotalRowCount = pDesc->iTotalRowCount;
	m_iVisibleRowCount = pDesc->iVisibleRowCount;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	Set_RowCounts(m_iTotalRowCount, m_iVisibleRowCount);
    return S_OK;
}


void CUIScrollButton::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

HRESULT CUIScrollButton::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

void CUIScrollButton::On_MouseDrag(const POINT& tMousePoint, const DIMOUSESTATE& tState)
{
	if (m_bGrabbed == false)
		return;
	CRect_Transform* pTransform = static_cast<CRect_Transform*>(m_pTransformCom);
	_float2 vMovedOffset = pTransform->Get_Offset();
	if (m_bDraggableX)
	{
		vMovedOffset.x = m_tGrabbedOffset.x + tMousePoint.x - m_tGrabbedMousePos.x;
	}
	if (m_bDraggableY)
	{
		vMovedOffset.y = m_tGrabbedOffset.y + tMousePoint.y - m_tGrabbedMousePos.y;
	}
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	vMovedOffset.y = std::clamp(vMovedOffset.y, m_fMinOffsetY, m_fMaxOffsetY - fSize.y);
	//현재 Y 위치에서 가장 가까운 Row의 Y 위치를 계산
	_float fScrollLength = m_fMaxOffsetY - m_fMinOffsetY - fSize.y;
	_float fCellLength = fScrollLength / (m_iTotalRowCount - m_iVisibleRowCount);
	_uint iCurrentRow = (_uint)((vMovedOffset.y - m_fMinOffsetY) / fCellLength);
	//iCurrentRow = min(m_iTotalRowCount - m_iVisibleRowCount, iCurrentRow);
	m_pScroller->Set_CursorRow(iCurrentRow);
}

void CUIScrollButton::Set_RowCounts(_uint iTotalRowCount, _uint iVisibleCount)
{
	m_iTotalRowCount = iTotalRowCount;
	m_iVisibleRowCount = iVisibleCount;
	_float fNewHeight = clamp(m_fMaxButtonSize * ((_float)iVisibleCount/(_float)iTotalRowCount ), m_fMinButtonSize, m_fMaxButtonSize);
	fNewHeight = std::clamp(fNewHeight, m_fMinButtonSize, m_fMaxButtonSize);
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	static_cast<CRect_Transform*>( m_pTransformCom)->Set_Size(fSize.x,fNewHeight);
}

void CUIScrollButton::Set_Offset(_uint iRow)
{
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	_float fScrollLength = m_fMaxOffsetY - m_fMinOffsetY - fSize.y;
	_float fCellLength;
	if (m_iTotalRowCount - m_iVisibleRowCount <= 0)
		fCellLength = fScrollLength / 1;
	else
		fCellLength = fScrollLength / (m_iTotalRowCount - m_iVisibleRowCount);

	_float fNewY = m_fMinOffsetY + fCellLength * iRow;
	fNewY = std::clamp(fNewY, m_fMinOffsetY, m_fMaxOffsetY - fSize.y);

	_float2 vCurOffset = static_cast<CRect_Transform*>(m_pTransformCom)->Get_Offset();
	static_cast<CRect_Transform*>(m_pTransformCom)->Set_Offset(vCurOffset.x, fNewY);
}

CUIScrollButton* CUIScrollButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIScrollButton* pInstance = new CUIScrollButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIScrollButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIScrollButton::Clone(void* pArg)
{
	CUIScrollButton* pInstance = new CUIScrollButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIScrollButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIScrollButton::Free()
{
	__super::Free();
}

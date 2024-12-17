#include "stdafx.h"
#include "UIButton.h"
#include "Texture.h"
#include "GameInstance.h"

CUIButton::CUIButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CUIButton::CUIButton(const CUIButton& Prototype)
	: CUIPanel(Prototype)
{
}

HRESULT CUIButton::Initialize_Prototype()
{
	return __super::Initialize_Prototype();
}

HRESULT CUIButton::Initialize(void* pArg)
{
	BUTTON_DESC* pDesc = static_cast<BUTTON_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL; 


	return S_OK;
}



HRESULT CUIButton::Ready_Components()
{
	return S_OK;
}



void CUIButton::On_MouseOver()
{
	if (m_bDisabled)return;
	__super::On_MouseOver();
}

void CUIButton::On_MouseEnter()
{
	if (m_bDisabled)return;
	__super::On_MouseEnter();
	m_iSRVIndex = m_arrSRVIndex[ BS_HIGHLIGHTED];
}

void CUIButton::On_MouseExit()
{
	if (m_bDisabled)return;
	__super::On_MouseExit();
	m_iSRVIndex = m_arrSRVIndex[BS_NORMAL];
		
}

void CUIButton::On_MouseLButtonDown(const POINT& tMousePoint)
{
	if (m_bDisabled)return;
	__super::On_MouseLButtonDown(tMousePoint);
	m_iSRVIndex = m_arrSRVIndex[BS_PRESSED];
	return;
}

void CUIButton::On_MouseLButtonUp()
{
	if (m_bDisabled)return;
	__super::On_MouseLButtonUp();
	m_iSRVIndex = m_arrSRVIndex[BS_NORMAL];
}

void CUIButton::On_MouseClick()
{
	if (m_bDisabled)return;
	__super::On_MouseClick();
	m_iSRVIndex = m_arrSRVIndex[BS_HIGHLIGHTED];

	for (auto& func : m_listCallback)
	{
		Call_Callback(func);
	}
	return ;
}

void CUIButton::On_MouseRightClick()
{
	if (m_bDisabled)return;
	__super::On_MouseRightClick();
	m_iSRVIndex = m_arrSRVIndex[BS_HIGHLIGHTED];

	for (auto& func : m_listRightClickCallback)
	{
		Call_Callback(func);
	}
	return;
}

void CUIButton::Set_Disable(_bool bValue)
{
	m_bDisabled = bValue;
	if (m_bDisabled)
	{
		m_iSRVIndex = m_arrSRVIndex[BS_DISABLED];
	}
	else
	{
		m_iSRVIndex = m_arrSRVIndex[BS_NORMAL];
	}
}

void CUIButton::Call_Callback(const function<void(void*)>& fCallback)
{
	if (m_bDisabled)return;
	fCallback(this);
}


CUIButton* CUIButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIButton* pInstance = new CUIButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIButton::Clone(void* pArg)
{
	CUIButton* pInstance = new CUIButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIButton::Free()
{
	__super::Free();
}

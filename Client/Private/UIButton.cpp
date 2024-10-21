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
	return S_OK;
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
}

void CUIButton::On_MouseEnter()
{
	m_iSRVIndex = BS_HIGHLIGHTED;
}

void CUIButton::On_MouseExit()
{
	m_iSRVIndex = BS_NORMAL;
		
}

void CUIButton::On_MouseLButtonDown()
{
	m_iSRVIndex = BS_PRESSED;
	return ;
}

void CUIButton::On_MouseLButtonUp()
{
	m_iSRVIndex = BS_NORMAL;
}

void CUIButton::On_MouseClick()
{
	m_iSRVIndex = BS_HIGHLIGHTED;

	for (auto& func : m_listCallback)
	{
		Call_Callback(func);
	}
	return ;
}

void CUIButton::Call_Callback(const function<void(void*)>& fCallback)
{
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

#include "stdafx.h"
#include "UIStatUpButton.h"
#include "PlayerInfo.h"

CUIStatUpButton::CUIStatUpButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIButton(pDevice, pContext)
{
}

CUIStatUpButton::CUIStatUpButton(const CUIStatUpButton& Prototype)
	:CUIButton(Prototype)
{
}

HRESULT CUIStatUpButton::Initialize(void* pArg)
{
	UISTATUPBUTTON_DESC* pDesc = static_cast<UISTATUPBUTTON_DESC*>(pArg);
	m_iStatType = pDesc->iStatType;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
    return S_OK;
}

void CUIStatUpButton::On_MouseClick()
{
	__super::On_MouseClick();
	PLAYERINIFO->Increase_Stat(m_iStatType);
}

CUIStatUpButton* CUIStatUpButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIStatUpButton* pInstance = new CUIStatUpButton(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CUIStatUpButton");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUIStatUpButton::Clone(void* pArg)
{
	CUIStatUpButton* pInstance = new CUIStatUpButton(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CUIStatUpButton");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIStatUpButton::Free()
{
	__super::Free();
}

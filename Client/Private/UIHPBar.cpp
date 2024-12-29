#include "stdafx.h"
#include "UIHPBar.h"
#include "Character.h"
#include "GameInstance.h"

CUIHPBar::CUIHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIBar(pDevice, pContext)
{
}

CUIHPBar::CUIHPBar(const CUIHPBar& Prototype)
	:CUIBar(Prototype)
{
}

HRESULT CUIHPBar::Initialize(void* pArg)
{
	HPBAR_DESC* pDesc = static_cast<HPBAR_DESC*>(pArg);
	m_pCharacter = pDesc->pCharacter;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CUIHPBar::Update(_float fTimeDelta)
{
	if (m_pCharacter)
		Update_Ratio(m_pCharacter->Get_HPRatio());
	__super::Update(fTimeDelta);
}

void CUIHPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}


CUIHPBar* CUIHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIHPBar* pInstance = new CUIHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIHPBar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIHPBar::Clone(void* pArg)
{
	CUIHPBar* pInstance = new CUIHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIHPBar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIHPBar::Free()
{
	__super::Free();
}

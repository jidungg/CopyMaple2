#include "stdafx.h"
#include "UIMonsterHPBar.h"
#include "Monster.h"
#include "GameInstance.h"

CUIHUDMonsterHPBar::CUIHUDMonsterHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIBar(pDevice, pContext)
{
}

CUIHUDMonsterHPBar::CUIHUDMonsterHPBar(const CUIHUDMonsterHPBar& Prototype)
	: CUIBar(Prototype)
{
}

HRESULT CUIHUDMonsterHPBar::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CUIHUDMonsterHPBar::Update(_float fTimeDelta)
{
	if (m_pMonster)
		Update_Ratio(m_pMonster->Get_HPRatio());
	__super::Update(fTimeDelta);
}

void CUIHUDMonsterHPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUIHUDMonsterHPBar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

void CUIHUDMonsterHPBar::Set_Monster(CMonster* pMonster)
{
	m_pMonster = pMonster;
}


CUIHUDMonsterHPBar* CUIHUDMonsterHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIHUDMonsterHPBar* pInstance = new CUIHUDMonsterHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CUIHUDMonsterHPBar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CUIHUDMonsterHPBar::Clone(void* pArg)
{
	CUIHUDMonsterHPBar* pInstance = new CUIHUDMonsterHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIHUDMonsterHPBar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUIHUDMonsterHPBar::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "UIQuickSlot.h"
#include "GameInstance.h"
CUIQuickSlot::CUIQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CUIQuickSlot::CUIQuickSlot(const CUIQuickSlot& Prototype)
	: CUIPanel(Prototype )
{
}

HRESULT CUIQuickSlot::Initialize_Prototype()
{
  	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, L"Texture_QuickSlot_Normal"));

	return S_OK;
}

HRESULT CUIQuickSlot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	QUICKLSLOT_DESC* pDesc = static_cast<QUICKLSLOT_DESC*>(pArg);
	m_eHotKey = pDesc->eHotKey;

	CUIPanel::PANEL_DESC panelDesc;
	panelDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	panelDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	panelDesc.fSizeX = 60;
	panelDesc.fSizeY = 60;
	panelDesc.fXOffset = 0;
	panelDesc.fYOffset = 0;
	m_pItemIcon = static_cast<CUIPanel*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &panelDesc));
	Add_OnlyTransformChild(m_pItemIcon);
	return S_OK;
}

void CUIQuickSlot::Update(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyState(m_eHotKey) == KEY_STATE::DOWN)
		On_MouseClick();
	__super::Update(fTimeDelta);
}

void CUIQuickSlot::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIQuickSlot::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_pShaderCom)
		m_pShaderCom->Begin(0);
	if (m_pVIBufferCom)
	{
		m_pVIBufferCom->Bind_BufferDesc();
		m_pVIBufferCom->Render();
	}

	if (m_pItemIcon)
		m_pItemIcon->Render();
	return S_OK;
}

void CUIQuickSlot::On_MouseClick()
{
	if (m_pQuickItem)
		m_pQuickItem->Use();
}

void CUIQuickSlot::Set_QuickItem(IQuickItem* pItem)
{
	UnSet_QuickItem();
	m_pQuickItem = pItem;
	string str = m_pQuickItem->Get_IconTag();
	wstring wstrIconTag(str.begin(), str.end());
	CTexture* pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, wstrIconTag));

	m_pItemIcon->Set_Texture(pTextureCom);

}

void CUIQuickSlot::UnSet_QuickItem()
{
	m_pItemIcon->Set_Texture(nullptr);
	m_pQuickItem = nullptr;
}

CUIQuickSlot* CUIQuickSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIQuickSlot* pInstance = new CUIQuickSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CQuickSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIQuickSlot::Clone(void* pArg)
{
	CUIQuickSlot* pInstance = new CUIQuickSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CQuickSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIQuickSlot::Free()
{
	__super::Free();
	Safe_Release(m_pItemIcon);
}

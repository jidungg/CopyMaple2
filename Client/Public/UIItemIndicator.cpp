#include "stdafx.h"
#include "UIItemIndicator.h"
#include "Item.h"
#include "GameInstance.h"


CUIItemIndicator::CUIItemIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)
{
}

CUIItemIndicator::CUIItemIndicator(const CUIItemIndicator& Prototype)
	: CUIPanel(Prototype), m_pItemDesc(Prototype.m_pItemDesc), m_pItemIcon(Prototype.m_pItemIcon)
{
}


HRESULT CUIItemIndicator::Initialize_Prototype(LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag)
{
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	if (FAILED(Add_Component(eBackTexProtoLev, szBackTexProtoTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CUIItemIndicator::Initialize(void* pArg)
{
	if(FAILED(CUIObject::Initialize(pArg)))
		return E_FAIL;


	/* Com_Texture */

	return S_OK;
}

HRESULT CUIItemIndicator::Render()
{

	__super::Render();
	return S_OK;
}

HRESULT CUIItemIndicator::On_ListItemDataSet(const UIListItemData* data)
{
	if (nullptr == data)
		return E_FAIL;
	m_pItemDesc = static_cast<const ITEM_DESC*>(data);

	CUIPanel::PANEL_DESC panelDesc{};
	panelDesc.eAnchorType = CORNOR_TYPE::CENTER;
	panelDesc.ePivotType = CORNOR_TYPE::CENTER;
	panelDesc.fXOffset = 0;
	panelDesc.fYOffset = 0;
	panelDesc.fSizeX = 70;
	panelDesc.fSizeY = 70;
	string strProtoItemIconTag = m_pItemDesc->strIconImageTag;
	wstring wstrItemIconTag(strProtoItemIconTag.begin(), strProtoItemIconTag.end());
	panelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, wstrItemIconTag, nullptr));

	m_pItemIcon =static_cast<CUIPanel*>( m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &panelDesc));
	if (FAILED(m_pItemIcon->Initialize(&panelDesc)))
		return E_FAIL;
	Add_Child(m_pItemIcon);

}




CUIItemIndicator* CUIItemIndicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag)
{
	CUIItemIndicator* pInstance = new CUIItemIndicator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eBackTexProtoLev, szBackTexProtoTag)))
	{
		MSG_BOX("Failed to Created : CUIItemIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIItemIndicator::Clone(void* pArg)
{
	CUIItemIndicator* pInstance = new CUIItemIndicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIItemIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIItemIndicator::Free()
{
	__super::Free();
}

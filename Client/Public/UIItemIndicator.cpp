#include "stdafx.h"
#include "UIItemIndicator.h"
#include "Item.h"
#include "GameInstance.h"


CUIItemIndicator::CUIItemIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIButton(pDevice, pContext)
{
}

CUIItemIndicator::CUIItemIndicator(const CUIItemIndicator& Prototype)
	: CUIButton(Prototype), m_pItemDesc(Prototype.m_pItemDesc), m_pItemIcon(Prototype.m_pItemIcon)
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

	if(FAILED(__super::Render()))
		return E_FAIL;
	m_pItemIcon->Render();
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
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	panelDesc.fSizeX = fSize.x - 12;
	panelDesc.fSizeY = fSize.y - 12;
	string strProtoItemIconTag = m_pItemDesc->strIconImageTag;
	wstring wstrItemIconTag(strProtoItemIconTag.begin(), strProtoItemIconTag.end());
	panelDesc.pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, wstrItemIconTag, nullptr));

	m_pItemIcon =static_cast<CUIPanel*>( m_pGameInstance->Clone_Proto_Object_Stock(CUIPanel::m_szProtoTag, &panelDesc));

	Add_FakeChild(m_pItemIcon);
}



void CUIItemIndicator::Call_Callback(const ButtonCallback& fCallback)
{
	fCallback(this);
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
	Safe_Release(m_pItemIcon);
}

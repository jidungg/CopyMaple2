#include "stdafx.h"
#include "UIItemIndicator.h"
#include "Item.h"
#include "GameInstance.h"
#include "UIIcon.h"


CUIButtonItemIndicator::CUIButtonItemIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIButton(pDevice, pContext)
{
}

CUIButtonItemIndicator::CUIButtonItemIndicator(const CUIButtonItemIndicator& Prototype)
	: CUIButton(Prototype)
	, m_pItemDesc(Prototype.m_pItemDesc)
	, m_pIcon(Prototype.m_pIcon)
{
}


HRESULT CUIButtonItemIndicator::Initialize_Prototype(LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag)
{
	__super::Initialize_Prototype();

	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_UI"),
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

HRESULT CUIButtonItemIndicator::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float3 fSize = m_pTransformCom->Compute_Scaled();
	CUIPanel::PANEL_DESC tPanelDesc{};

	tPanelDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tPanelDesc.ePivotType = CORNOR_TYPE::CENTER;
	tPanelDesc.fSizeX = fSize.x - 12;
	tPanelDesc.fSizeY = fSize.y - 12;
	tPanelDesc.fXOffset = 0;
	tPanelDesc.fYOffset = 0;
	m_pIcon = static_cast<CUIIcon*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIIcon::m_szProtoTag, &tPanelDesc));
	Add_Child(m_pIcon);
	return S_OK;
}

void CUIButtonItemIndicator::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

HRESULT CUIButtonItemIndicator::Render()
{
 	if(FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIButtonItemIndicator::Render_ListEntry()
{
	if (Is_Valid())
		return Render();
	return S_OK;
}

HRESULT CUIButtonItemIndicator::On_ListItemDataSet(const ITEM_DATA* data)
{
	if (nullptr == data)
		return E_FAIL;
	m_pItemDesc = data;
	CTexture* pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, m_pItemDesc->szIconImageTag, nullptr));
	m_pIcon->Set_Texture(pTexture);
	m_pIcon->Set_Active(true);
	Set_Disable(false);
}

void CUIButtonItemIndicator::On_CreateListItemEntry(CUIList* pList, _uint iIndex)
{
	__super::On_CreateListItemEntry(pList, iIndex);
	Set_Disable(true);
}

void CUIButtonItemIndicator::Set_ListItemEntryActive(_bool bActive)
{
	Set_Active(bActive);
}




void CUIButtonItemIndicator::Call_Callback(const ButtonCallback& fCallback)
{
	fCallback(this);
}





CUIButtonItemIndicator* CUIButtonItemIndicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eBackTexProtoLev, const _tchar* szBackTexProtoTag)
{
	CUIButtonItemIndicator* pInstance = new CUIButtonItemIndicator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eBackTexProtoLev, szBackTexProtoTag)))
	{
		MSG_BOX("Failed to Created : CUIItemIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIButtonItemIndicator::Clone(void* pArg)
{
	CUIButtonItemIndicator* pInstance = new CUIButtonItemIndicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUIItemIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIButtonItemIndicator::Free()
{
	__super::Free();

}

void CUIButtonItemIndicator::Set_Offset(_float iX, _float iY)
{
	static_cast<CRect_Transform*>(m_pTransformCom)->Set_Offset(iX, iY);
}

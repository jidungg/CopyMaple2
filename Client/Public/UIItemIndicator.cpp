#include "stdafx.h"
#include "UIItemIndicator.h"
#include "Item.h"
#include "GameInstance.h"


CUIItemIndicator::CUIItemIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIButton(pDevice, pContext)
{
}

CUIItemIndicator::CUIItemIndicator(const CUIItemIndicator& Prototype)
	: CUIButton(Prototype), m_pItemDesc(Prototype.m_pItemDesc)
	, m_pIconTexure(Prototype.m_pIconTexure)
	, m_pIconTransform(Prototype.m_pIconTransform)
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
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	/* Com_Texture */

	return S_OK;
}

HRESULT CUIItemIndicator::Render()
{
	if(FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pIconTransform->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pIconTexure->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_BorderSize", &m_vIconBorder, sizeof(XMUINT4))))
		return E_FAIL;
	_float4 vMinMax = static_cast<CRect_Transform*>(m_pIconTransform)->Get_MinMax();
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MinMax", &vMinMax, sizeof(XMUINT4))))
		return E_FAIL;
	if (m_pShaderCom)
		m_pShaderCom->Begin(0);
	if (m_pVIBufferCom)
	{
		m_pVIBufferCom->Bind_BufferDesc();
		m_pVIBufferCom->Render();
	}
	return S_OK;
}

HRESULT CUIItemIndicator::On_ListItemDataSet(const UIListItemData* data)
{
	if (nullptr == data)
		return E_FAIL;
	m_pItemDesc = static_cast<const ITEM_DATA*>(data);

	CRect_Transform::RECTTRANSFORM_DESC tRectDesc{};
	tRectDesc.eAnchorType = CORNOR_TYPE::CENTER;
	tRectDesc.ePivotType = CORNOR_TYPE::CENTER;
	tRectDesc.fXOffset = 0;
	tRectDesc.fYOffset = 0;
	_float3 fSize = m_pTransformCom->Compute_Scaled();
	tRectDesc.fSizeX = fSize.x - 12;
	tRectDesc.fSizeY = fSize.y - 12;
	m_pIconTransform = static_cast<CRect_Transform*>( CRect_Transform::Create(m_pDevice, m_pContext));
	m_pIconTransform->Set_Parent(m_pTransformCom);
	m_pIconTransform->Initialize(&tRectDesc);
	string strProtoItemIconTag = m_pItemDesc->strIconImageTag;
	wstring wstrItemIconTag(strProtoItemIconTag.begin(), strProtoItemIconTag.end());
	m_pIconTexure = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, wstrItemIconTag, nullptr));
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
	Safe_Release(m_pIconTexure);
	Safe_Release(m_pIconTransform);
}

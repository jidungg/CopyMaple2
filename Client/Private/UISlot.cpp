#include "stdafx.h"
#include "UISlot.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"
#include "UIIcon.h"
#include "GameInstance.h"

CUISlot::CUISlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPanel(pDevice, pContext)

{
}

CUISlot::CUISlot(const CUISlot& Prototype)
	: CUIPanel(Prototype)
{
}

HRESULT CUISlot::Initialize(void* pArg)
{
	if (FAILED(CUIPanel::Initialize(pArg)))
		return E_FAIL;

	UISLOT_DESC* pDesc = static_cast<UISLOT_DESC*>(pArg);

	CUIIcon::PANEL_DESC tIconDesc;
	tIconDesc.eAnchorType = pDesc->tIconDesc.eAnchorType;
	tIconDesc.ePivotType = pDesc->tIconDesc.ePivotType;
	tIconDesc.fSizeX = pDesc->tIconDesc.fSizeX;
	tIconDesc.fSizeY = pDesc->tIconDesc.fSizeY;
	tIconDesc.fXOffset = pDesc->tIconDesc.fXOffset;
	tIconDesc.fYOffset = pDesc->tIconDesc.fYOffset; 
	tIconDesc.pTextureCom = pDesc->pTextureCom;
	
	m_pIcon = static_cast<CUIIcon*>( m_pGameInstance->Clone_Proto_Object_Stock(CUIIcon::m_szProtoTag, &tIconDesc));
	Add_Child(m_pIcon);

    return S_OK;
}

HRESULT CUISlot::Render()
{

	return __super::Render();
}

void CUISlot::Set_IconTexture(CTexture* pTexture)
{
	m_pIcon->Set_Texture(pTexture);
	
}

void CUISlot::Compute_Matrix()
{
	__super::Compute_Matrix();

}



void CUISlot::Free()
{
	__super::Free();

}

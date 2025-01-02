#include "stdafx.h"
#include "UIQuickSlot.h"
#include "GameInstance.h"
#include "SlotItem.h"
#include "UIFont.h"

map<KEY, const _tchar*> mapHotKeyText
{
	{KEY::Q, TEXT("Q")},
	{KEY::W, TEXT("W")},
	{KEY::E, TEXT("E")},
	{KEY::R, TEXT("R")},
	{KEY::A, TEXT("A")},
	{KEY::S, TEXT("S")},
	{KEY::D, TEXT("D")},
	{KEY::F, TEXT("F")},
	{KEY::NUM1, TEXT("1")},
	{KEY::NUM2, TEXT("2")},
	{KEY::NUM3, TEXT("3")},
	{KEY::NUM4, TEXT("4")},
	{KEY::NUM5, TEXT("5")},
	{KEY::NUM6, TEXT("6")},
	{KEY::NUM7, TEXT("7")},
	{KEY::NUM8, TEXT("8")}
};
CUIQuickSlot::CUIQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUISlot(pDevice, pContext)
{
}

CUIQuickSlot::CUIQuickSlot(const CUIQuickSlot& Prototype)
	: CUISlot(Prototype )
	, m_pQuickItem(Prototype.m_pQuickItem)
{
}

HRESULT CUIQuickSlot::Initialize_Prototype()
{
  	m_pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, L"Texture_QuickSlot_Normal"));

	return S_OK;
}

HRESULT CUIQuickSlot::Initialize(void* pArg)
{
	QUICKLSLOT_DESC* pDesc = static_cast<QUICKLSLOT_DESC*>(pArg);
	m_eHotKey = pDesc->eHotKey;

	pDesc->tIconDesc.eAnchorType = CORNOR_TYPE::CENTER;
	pDesc->tIconDesc.ePivotType = CORNOR_TYPE::CENTER;
	pDesc->tIconDesc.fSizeX = pDesc->fSizeX ;
	pDesc->tIconDesc.fSizeY = pDesc->fSizeY ;
	pDesc->tIconDesc.fXOffset = 0;
	pDesc->tIconDesc.fYOffset = 0;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	CUIFont::UIFontDesc tFontDesc{};
	tFontDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	tFontDesc.fXOffset = 2.5;
	tFontDesc.fYOffset = 2.5;
	tFontDesc.pFontTag = L"LV2Gothic_Medium_10";
	tFontDesc.vColor = _vector{ 1.f, 1.f, 1.f, 1.f };
	tFontDesc.bShade = true;
	tFontDesc.pText = mapHotKeyText[m_eHotKey];
	m_pHotKeyFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pHotKeyFont);

	tFontDesc.eAnchorType = CORNOR_TYPE::RIGHT_BOT;
	tFontDesc.ePivotType = CORNOR_TYPE::RIGHT_BOT;
	tFontDesc.fXOffset = -2.5;
	tFontDesc.fYOffset = -2.5;
	tFontDesc.pFontTag = L"LV2Gothic_Medium_10";
	tFontDesc.vColor = _vector{ 1.f, 1.f, 1.f, 1.f };
	tFontDesc.bShade = true;
	m_pStackCountFont = static_cast<CUIFont*>(m_pGameInstance->Clone_Proto_Object_Stock(CUIFont::m_szProtoTag, &tFontDesc));
	Add_Child(m_pStackCountFont);
	m_pStackCountFont->Set_Active(false);

	return S_OK;
}

void CUIQuickSlot::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);
	if(m_pQuickItem)
		m_pQuickItem->Update_Cool(fTimeDelta);
}

HRESULT CUIQuickSlot::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	_float fCoolTImeRatio = 0.f;
	if (m_pQuickItem) //0~1
		fCoolTImeRatio =  m_pQuickItem->Get_CoolTimeRatio();
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_pShaderCom)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fVerticalRatio", &fCoolTImeRatio, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(4)))
			return E_FAIL;
	}
	if (m_pVIBufferCom)
	{
		if (FAILED(m_pVIBufferCom->Bind_BufferDesc()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

	}

	return S_OK;
}




void CUIQuickSlot::Receive_Input()
{
	if (m_pQuickItem)
	{
		if (1 > m_pQuickItem->Get_CoolTimeRatio() )
			return;
		m_pQuickItem->Use();
		_uint iStackCount = m_pQuickItem->Get_StackCount_Quick();
		if (0 >= iStackCount)
			Set_QuickItem(nullptr);
		else
			Set_StackCount(iStackCount);
	}
}

void CUIQuickSlot::On_MouseClick()
{
	if (m_pQuickItem)
	{
		if (1 > m_pQuickItem->Get_CoolTimeRatio())
			return;
		m_pQuickItem->Use();
		_uint iStackCount = m_pQuickItem->Get_StackCount_Quick();
		if (0 >= iStackCount)
			Set_QuickItem(nullptr);
		else
			Set_StackCount(iStackCount);
	}
}

void CUIQuickSlot::Set_QuickItem(IQuickItem* pItem)
{
	m_pQuickItem = pItem;
	if (pItem == nullptr)
	{
		Set_IconTexture(nullptr);
		m_pStackCountFont->Set_Active(false);
	}
	else
	{
		string str = m_pQuickItem->Get_IconTag();
		wstring wstrIconTag(str.begin(), str.end());
		CTexture* pTextureCom = static_cast<CTexture*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVELID::LEVEL_LOADING, wstrIconTag));

		Set_IconTexture(pTextureCom);
		_int iCount = m_pQuickItem->Get_StackCount_Quick();
		if(iCount >= 0)
		{
			m_pStackCountFont->Set_Active(true);
			Set_StackCount(iCount);
		}
		else
		{
			m_pStackCountFont->Set_Active(false);
		}
	}
}

void CUIQuickSlot::Set_StackCount(_uint iCount)
{
	wstring str = to_wstring(iCount);
	m_pStackCountFont->Set_Text(str.data());
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

}

bool CUIQuickSlot::Has_Item()
{
	return m_pQuickItem != nullptr;
}

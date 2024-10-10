#include "UIObject.h"

#include "GameInstance.h"

_uint CUIObject::m_iStaticPriority = 0;

CUIObject::CUIObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject & Prototype)
	: CGameObject{ Prototype },
	m_pTextureCom{ Prototype.m_pTextureCom },
	m_pVIBufferCom{ Prototype.m_pVIBufferCom },
	m_pShaderCom{ Prototype.m_pShaderCom }
{
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBufferCom);
	Safe_AddRef(m_pShaderCom);
	Increase_Priority();
}

HRESULT CUIObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUIObject::Initialize(void * pArg)
{
	UIOBJECT_DESC*	pDesc = static_cast<UIOBJECT_DESC*>(pArg);
	if (nullptr != pDesc)
	{
		m_pTarget = pDesc->pTarget;
		Safe_AddRef(m_pTarget);
		m_pTextureCom = pDesc->pTextureCom;
	}


	m_pTransformCom = CRect_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

//부모의 Position과 자신의 Pivot을 이용하여 자신의 위치를 계산한다.
void CUIObject::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CUIObject::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive) return;

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}


HRESULT CUIObject::Render()
{
	if (false == m_bActive) return S_OK;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (m_pShaderCom)
		m_pShaderCom->Begin(0);
	if (m_pVIBufferCom)
	{
		m_pVIBufferCom->Bind_BufferDesc();
		m_pVIBufferCom->Render();
	}

	for (auto& child : m_pChilds)
		child->Render();

	return S_OK;
}

void CUIObject::Add_Child(CGameObject* pChild)
{
	__super::Add_Child(pChild);
	static_cast<CRect_Transform*>(pChild->Get_Transform())->Compute_Matrix();
}

void CUIObject::Increase_Priority()
{
	if(m_pParent == nullptr)
		m_iPriority = m_iStaticPriority++;
	else
		static_cast<CUIObject*>(m_pParent)->Increase_Priority();
}


HRESULT CUIObject::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (m_pShaderCom)
	{
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_UI_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_UI_PROJ))))
			return E_FAIL;
	}
	if(m_pTextureCom)
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iSRVIndex)))
			return E_FAIL;

	return S_OK;
}


void CUIObject::MouseOver()
{
	if (m_bMouseOver == false)
		On_MouseEnter();
	else
		On_MouseOver();
	m_bMouseOver = true;

}
void CUIObject::MouseNotOver()
{
	if (m_bMouseOver == true)
	{
		On_MouseExit();
	}
	m_bMouseOver = false;
}
void CUIObject::On_MouseEnter()
{
}


void CUIObject::On_MouseOver()
{
}

void CUIObject::On_MouseExit()
{
}

bool CUIObject::Consume_MouseLButtonDown()
{
	return false;
}

void CUIObject::On_MouseLButtonUp()
{
	return;
}

bool CUIObject::Consume_MouseRButtonDown()
{
	return false;
}

void CUIObject::On_MouseRButtonUp()
{
	return;
}

bool CUIObject::Consume_MouseClick()
{
	return false;
}

bool CUIObject::Consume_MouseRightClick()
{
	return false;
}

bool CUIObject::Check_MouseOver(POINT fPos)
{
	return static_cast<CRect_Transform*>( m_pTransformCom)->Is_InRect(_float2((float)fPos.x, (float)fPos.y));
}

CUIObject* CUIObject::Find_FocusedUI(POINT fPos)
{

	if (false == Check_MouseOver(fPos))
	{
		MouseNotOver();
		return nullptr;
	}

	for (auto& child : m_pChilds)
	{
		if (false == child->Is_Active()) continue;

		CUIObject* pChildUI = static_cast<CUIObject*>(child);
		CUIObject* pFocusedUI = pChildUI->Find_FocusedUI(fPos);
		if (pFocusedUI != nullptr)
			return pFocusedUI;
	}

	return this;
}



void CUIObject::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
  	Safe_Release(m_pVIBufferCom);
}

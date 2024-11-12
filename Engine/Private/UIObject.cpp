#include "UIObject.h"

#include "GameInstance.h"

_uint CUIObject::m_iStaticPriority = 0;

CUIObject::CUIObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject & Prototype)
	: CGameObject{ Prototype }
{

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

	}


	m_pTransformCom = CRect_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;
	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

//�θ��� Position�� �ڽ��� Pivot�� �̿��Ͽ� �ڽ��� ��ġ�� ����Ѵ�.
void CUIObject::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}


void CUIObject::Add_Child(CGameObject* pChild)
{
	__super::Add_Child(pChild);

	static_cast<CUIObject*>(pChild)->m_pParent = this;

	Compute_Matrix_Recursive();
}

void CUIObject::Remove_Child(CGameObject* pChild)
{
	__super::Remove_Child(pChild);
	static_cast<CUIObject*>(pChild)->m_pParent = nullptr;
}
void CUIObject::Increase_Priority()
{
	if(m_pParent == nullptr)
		m_iPriority = m_iStaticPriority++;
	else
		static_cast<CUIObject*>(m_pParent)->Increase_Priority();
}

void CUIObject::Compute_Matrix_Recursive()
{
	Compute_Matrix();
	for (auto& p : m_pChilds)
	{
		static_cast<CUIObject*>(p)->Compute_Matrix_Recursive();
	}
}

void CUIObject::Compute_Matrix()
{
	static_cast<CRect_Transform*>(m_pTransformCom)->Compute_Matrix();
	__super::Compute_Matrix();
}





void CUIObject::MouseOver()
{
	if (m_bMouseOver == false)
		On_MouseEnter();
	else
		On_MouseOver();
	m_bMouseOver = true;

}
void CUIObject::MouseNotOver(CUIObject* pExcept)
{
	if (m_bMouseOver == true)
	{
		On_MouseExit();
	}
	m_bMouseOver = false;
	for (auto& i : m_pChilds)
	{
		if (i != pExcept) 
			static_cast<CUIObject*>( i)->MouseNotOver(pExcept);
	}
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

void CUIObject::On_MouseLButtonDown()
{
	return ;
}

void CUIObject::On_MouseLButtonUp()
{
	return;
}

void CUIObject::On_MouseRButtonDown()
{
	return ;
}

void CUIObject::On_MouseRButtonUp()
{
	return;
}

void CUIObject::On_MouseClick()
{
	return ;
}

void CUIObject::On_MouseRightClick()
{
	return ;
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
	MouseOver();
	CUIObject* pFocusedUI = nullptr;
	for (auto& child : m_pChilds)
	{
		if (false == child->Is_Active()) continue;
		CUIObject* pChildUI = static_cast<CUIObject*>(child);
		pFocusedUI = pChildUI->Find_FocusedUI(fPos);
		if (pFocusedUI != nullptr)
			return pFocusedUI;
	}

	return this;
}



void CUIObject::Free()
{
	__super::Free();


}

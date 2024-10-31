#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Transform.h"
#include "Collider.h"
_uint CGameObject::m_iObjCount = 0;
CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	m_iObjID = m_iObjCount++;
}

CGameObject::CGameObject(const CGameObject & Prototype) 
	: m_pDevice{ Prototype.m_pDevice }
	, m_pContext{ Prototype.m_pContext }
	, m_pGameInstance{ Prototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	m_iObjID = m_iObjCount++;
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		GAMEOBJECT_DESC*		pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
		m_pTarget = pDesc->pTarget;
		Safe_AddRef(m_pTarget);
	}	

	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
	for (auto& c : m_Components)
	{
		if (c.second->Is_Active())
			c.second->Priority_Update(fTimeDelta);
	}
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active())
			child->Priority_Update(fTimeDelta);
	}
}

void CGameObject::Update(_float fTimeDelta)
{
	for (auto& c : m_Components)
	{
		if (c.second->Is_Active())
			c.second->Update(fTimeDelta);
	}
	Compute_Matrix();
	for (auto& child : m_pChilds)
	{
		if ( child->Is_Active())
			child->Update(fTimeDelta);
	}
}
void CGameObject::Compute_Matrix()
{

	if (m_pParentMatrix != nullptr)
		XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));
	else
		XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix());

}


void CGameObject::Late_Update(_float fTimeDelta)
{
	for (auto& c : m_Components)
	{
		if (c.second->Is_Active())
			c.second->Late_Update(fTimeDelta);
	}
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active())
			child->Late_Update(fTimeDelta);
	}
}

void CGameObject::Final_Update()
{
	for (auto& m_pChilds_iter = m_pChilds.begin(); m_pChilds_iter != m_pChilds.end();)
	{
		if ((*m_pChilds_iter)->Is_Dead())
		{
			Safe_Release(*m_pChilds_iter);
			m_pChilds_iter = m_pChilds.erase(m_pChilds_iter);
		}
		else
		{
			(*m_pChilds_iter)->Final_Update();
			++m_pChilds_iter;
		}
	}
}

HRESULT CGameObject::Render()
{
	if (false == m_bActive) return S_OK;
	for (auto& child : m_pChilds)
	{

		if (child->Is_Active())
			child->Render();
	}
	return S_OK;
}

void CGameObject::Add_Child(CGameObject* pChild)
{
	if (pChild == nullptr)
		return;
	m_pChilds.push_back(pChild);
	pChild->m_pParentMatrix = m_pTransformCom->Get_WorldFloat4x4_Ptr();
	pChild->Get_Transform()->Set_Parent(m_pTransformCom);
}

void CGameObject::Remove_Child(CGameObject* pChild)
{
	if (pChild == nullptr)
		return;

	m_pChilds.remove(pChild);
	pChild->m_pParentMatrix = nullptr;
	pChild->Get_Transform()->Set_Parent(nullptr);
}

CComponent * CGameObject::Find_Component(const _wstring & strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);

	if(iter == m_Components.end())
		return nullptr;

	return iter->second;
}

bool CGameObject::Check_Collision(const Ray& tRay, RaycastHit* pOut)
{
	CCollider* pCollider = nullptr;
	pCollider = static_cast<CCollider*>(Find_Component(CCollider::m_szCompTag));
	bool bIsHit = false;
	RaycastHit tMinHit;
	tMinHit.fDist = 9999;
	if (pCollider != nullptr && pCollider->Is_Active())
	{
		if (true == pCollider->RayCast(tRay, pOut))
		{
			bIsHit = true;
			if (pOut->fDist < tMinHit.fDist)
			{
				tMinHit = *pOut;
			}
		}
	}

	for (auto& child : m_pChilds)
	{
		if (child->Is_Active() == false || child->Is_Dead())
			continue;
		if (child->Check_Collision(tRay, pOut))
		{
			bIsHit = true;
			if (pOut->fDist < tMinHit.fDist)
			{
				tMinHit = *pOut;
			}
		}
	}
	if (bIsHit)
	{
		*pOut  = tMinHit;
	}
	return bIsHit;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, const _wstring & strComponentTag, CComponent** ppOut, void * pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;


	CComponent*		pComponent = static_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, iPrototypeLevelIndex, strPrototypeTag, pArg));

	*ppOut = pComponent;

	return Add_Component(pComponent, strComponentTag);
}

HRESULT CGameObject::Add_Component(CComponent* pComponent, const _wstring& strComponentTag)
{
	if (nullptr == pComponent)
		return E_FAIL;
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);
	pComponent->Set_Owner(this);

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CGameObject::Remove_Component(CComponent* pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(nullptr);
	for (auto& comp : m_Components)
	{
		if (comp.second == pComponent)
		{
			Safe_Release(comp.second);
			m_Components.erase(comp.first);
			return S_OK;
		}
	}
	return E_FAIL;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	for (auto& i : m_pChilds)
	{
		Safe_Release(i);
	}
 	m_pChilds.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pTarget);
}

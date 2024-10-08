#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Transform.h"

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

/* 게임내에서 직접 사용되기 위한 사본 객체가 생성되낟. */
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
	if (false == m_bActive) return;
	for (auto& c : m_Components)
	{
		if (c.second != nullptr && c.second->Is_Active())
			c.second->Priority_Update(fTimeDelta);
	}
	for (auto& child : m_pChilds)
	{
		if (child != nullptr && child->Is_Active())
			child->Priority_Update(fTimeDelta);
	}
}

void CGameObject::Update(_float fTimeDelta)
{
	if (false == m_bActive) return;
	for (auto& c : m_Components)
	{
		if (c.second != nullptr && c.second->Is_Active())
			c.second->Update(fTimeDelta);
	}
	for (auto& child : m_pChilds)
	{
		if (child != nullptr && child->Is_Active())
			child->Update(fTimeDelta);
	}
}	

void CGameObject::Late_Update(_float fTimeDelta)
{
	if (false == m_bActive) return;
	for (auto& c : m_Components)
	{
		if (c.second != nullptr && c.second->Is_Active())
			c.second->Late_Update(fTimeDelta);
	}
	for (auto& child : m_pChilds)
	{
		if (child != nullptr && child->Is_Active())
			child->Late_Update(fTimeDelta);
	}
}

HRESULT CGameObject::Render()
{
	if (false == m_bActive) return S_OK;
	return S_OK;
}

void CGameObject::Add_Child(CGameObject* pChild)
{
	if (pChild == nullptr)
		return;
	m_pChilds.push_back(pChild);

	pChild->m_pParent = this;
	pChild->Get_Transform()->Set_Parent(m_pTransformCom);
}

CComponent * CGameObject::Find_Component(const _wstring & strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);

	if(iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, const _wstring & strComponentTag, CComponent** ppOut, void * pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	CComponent*		pComponent = static_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, iPrototypeLevelIndex, strPrototypeTag, pArg));

	*ppOut = pComponent;

	return Add_Component(pComponent, strComponentTag, pArg);
}

HRESULT CGameObject::Add_Component(CComponent* pComponent, const _wstring& strComponentTag, void* pArg)
{
	if (nullptr == pComponent)
		return E_FAIL;
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();

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

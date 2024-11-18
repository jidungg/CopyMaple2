#include "stdafx.h"
#include "Portal.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "LevelChangeEvent.h"
#include "Collider_AABB.h"

CPortal::CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInteractableObject(pDevice, pContext)
{
}

CPortal::CPortal(const CPortal& Prototype)
	: CInteractableObject(Prototype)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CPortal::Initialize(void* pArg)
{
	PORTAL_DESC* pDesc = (PORTAL_DESC*)pArg;
	m_eLevelID = pDesc->eLevelID;
	pDesc->eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(pDesc->strModelProtoName, "he_fi_cubric_bush_a01.model");
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//Collider
	CCollider_AABB::AABB_COLLIDER_DESC tDesc{};
	tDesc.vCenter = { 0,0.5f,0 };
	tDesc.vExtentes = { 0.5f,0.5f,0.5f };
	m_vecCollider.resize(1,nullptr);
	if (FAILED(Add_Component(LEVEL_LOADING, CCollider_AABB::m_szProtoTag,
		CColliderBase::m_szCompTag, reinterpret_cast<CComponent**>(&m_vecCollider[0]), &tDesc)))
		return E_FAIL;

	return S_OK;
}

void CPortal::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

_bool CPortal::Check_Collision(CGameObject* pOther)
{
	m_vecCollider[0]->Update(XMLoadFloat4x4(&m_WorldMatrix));
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
	return m_vecCollider[0]->Intersects(pOther->Get_Collider(0));
}

HRESULT CPortal::Render()
{

	m_vecCollider[0]->Render();
	return S_OK;
}



void CPortal::Interact(CPlayer* pActor)
{

	m_pGameInstance->Push_Event(CLevelChangeEvent::Create(CLevel_Loading::Create(m_pDevice, m_pContext, m_eLevelID)));
}

_bool CPortal::Is_InteractionPossible(CPlayer* pActor)
{
	return true;
}

CPortal* CPortal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPortal* pInstance = new CPortal(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed CPortal ");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
	CPortal* pInstance = new CPortal(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Failed CPortal ");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}





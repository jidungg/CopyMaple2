#include "stdafx.h"
#include "AttachableBodyPart.h"
#include "GameInstance.h"
#include "Player.h"


CAttachableBodyPart::CAttachableBodyPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CInteractableObject(pDevice, pContext)
{
}

CAttachableBodyPart::CAttachableBodyPart(const CInteractableObject& Prototype)
	:CInteractableObject(Prototype)
{
}

HRESULT CAttachableBodyPart::Initialize(void* pArg)
{
	ATTACHABLE_BODYPART_DESC* pDesc = (ATTACHABLE_BODYPART_DESC*)pArg;

	pDesc->fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	

	m_pParentMatrix = pDesc->pParentMatrix;
	m_pSocketMatrix = pDesc->pSocketMatrix;

	//Collider
	m_vecCollider.resize(1, nullptr);
	if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Sphere::m_szProtoTag,
		CColliderBase::m_szCompTag, reinterpret_cast<CComponent**>(&m_vecCollider[0]), &pDesc->tColliderDesc)))
		return E_FAIL;

	return S_OK;
}
void CAttachableBodyPart::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONLIGHT, this);
}
HRESULT CAttachableBodyPart::Render()
{

	//m_vecCollider[0]->Render();
	return __super::Render();
}
void CAttachableBodyPart::Interact(CPlayer* pActor)
{
	pActor->Attach_To(this);
	m_setAttachedPlayer.insert(pActor);

}
void CAttachableBodyPart::Detach_Players()
{
	for (auto& player : m_setAttachedPlayer)
		player->Detach_From();
	m_setAttachedPlayer.clear();
}
void CAttachableBodyPart::RemovePlayer(CPlayer* pPlayer)
{
	m_setAttachedPlayer.erase(pPlayer);
}
_float CAttachableBodyPart::Get_Radius()
{
	return static_cast<CCollider_Sphere*>(m_vecCollider[0])->Get_Desc()->Radius;
}
_bool CAttachableBodyPart::Is_Attached(CPlayer* pPlayer)
{
	return m_setAttachedPlayer.find(pPlayer) != m_setAttachedPlayer.end();
}
_bool CAttachableBodyPart::Is_InteractionPossible(CPlayer* pActor)
{
	return m_setAttachedPlayer.find(pActor) == m_setAttachedPlayer.end();
}

void CAttachableBodyPart::Compute_Matrix()
{
	assert(nullptr != m_pParentMatrix);
	assert(nullptr != m_pSocketMatrix);
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	m_vecCollider[0]->Update(XMLoadFloat4x4( &m_WorldMatrix));
}



CAttachableBodyPart* CAttachableBodyPart::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAttachableBodyPart* pInstance = new CAttachableBodyPart(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed CAttachableBodyPart ");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}
CGameObject* CAttachableBodyPart::Clone(void* pArg)
{
	CAttachableBodyPart* pInstance = new CAttachableBodyPart(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Failed CAttachableBodyPart ");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CAttachableBodyPart::Free()
{
	__super::Free();
}



#include "stdafx.h"
#include "Bullet_BayarAttackD.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModel.h"
#include "Skill.h"

CBullet_BayarAttackD::CBullet_BayarAttackD(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CBullet_BayarAttackD::CBullet_BayarAttackD(const CBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CBullet_BayarAttackD::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	tDesc.fRadius = 0.8f;
	tDesc.vCenter = { -0.4f ,0.4f,0.f };
	m_pCollider = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));


	m_pParentMatrix = m_pShooter->Get_Transform()->Get_WorldFloat4x4_Ptr();
	m_pSocketMatrix = static_cast<CCharacter*>(m_pShooter)->Get_BoneMatrix(m_szAttachBoneName);

	return S_OK;
}

void CBullet_BayarAttackD::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	m_pCollider->Update(XMLoadFloat4x4(&m_WorldMatrix));
}

void CBullet_BayarAttackD::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);
	for (auto& pTarget : listTarget)
	{
		if (m_setHitObject.find(pTarget) != m_setHitObject.end())
			continue;
		m_setHitObject.insert(pTarget);
		m_pGameInstance->Push_Event(CHitEvent::Create(m_pShooter, pTarget, (_int)m_fDamage, m_szHitEffectTag));
	}

}

HRESULT CBullet_BayarAttackD::Render()
{
	m_pCollider->Render();
	return __super::Render();
}

void CBullet_BayarAttackD::Compute_Matrix()
{
	assert(nullptr != m_pParentMatrix);
	assert(nullptr != m_pSocketMatrix);
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

}

void CBullet_BayarAttackD::Launch(_float fDamage)
{
	__super::Launch(fDamage);
	m_setHitObject.clear();
	m_pCollider->Set_Active(true);
}

CBullet_BayarAttackD* CBullet_BayarAttackD::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_BayarAttackD* pInstance = new CBullet_BayarAttackD(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_BayarAttackD");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_BayarAttackD::Clone(void* pArg)
{
	CBullet_BayarAttackD* pInstance = new CBullet_BayarAttackD(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_BayarAttackD");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_BayarAttackD::Free()
{
	__super::Free();
}

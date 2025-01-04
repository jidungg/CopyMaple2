#include "stdafx.h"
#include "Bullet_BayarAttackB.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModel.h"
#include "Skill.h"


CBullet_BayarAttackB::CBullet_BayarAttackB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CBullet_BayarAttackB::CBullet_BayarAttackB(const CBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CBullet_BayarAttackB::Initialize(void* pArg)
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

void CBullet_BayarAttackB::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	m_pCollider->Update(XMLoadFloat4x4(& m_WorldMatrix));
}

void CBullet_BayarAttackB::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);
	for (auto& pTarget : listTarget)
	{
		if (m_setHitObject.find(pTarget) != m_setHitObject.end())
			continue;
		m_setHitObject.insert(pTarget);
		_bool bCrit;
		_float fDamage = m_pSkill->Calc_Damg(bCrit);
		m_pGameInstance->Push_Event(CDamgEvent::Create(m_pShooter, pTarget, (_int)fDamage, bCrit,false, m_eHitEffect));
	}


}

HRESULT CBullet_BayarAttackB::Render()
{
	m_pCollider->Render();
	return __super::Render();
}

void CBullet_BayarAttackB::Compute_Matrix()
{
	assert(nullptr != m_pParentMatrix);
	assert(nullptr != m_pSocketMatrix);
	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

}

void CBullet_BayarAttackB::Launch(CSkill* pSkill)
{
	__super::Launch(pSkill);
	m_setHitObject.clear();
	m_pCollider->Set_Active(true);
}


CBullet_BayarAttackB* CBullet_BayarAttackB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_BayarAttackB* pInstance = new CBullet_BayarAttackB(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_BayarAttackB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_BayarAttackB::Clone(void* pArg)
{
	CBullet_BayarAttackB* pInstance = new CBullet_BayarAttackB(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_BayarAttackB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_BayarAttackB::Free()
{
	__super::Free();
}

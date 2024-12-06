#include "stdafx.h"
#include "Bullet_WildFire.h"
#include "GameInstance.h"
#include "Collider_Cylinder.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModel.h"

CBullet_WildFire::CBullet_WildFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CBullet_WildFire::CBullet_WildFire(const CBullet_WildFire& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CBullet_WildFire::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CCollider_Cylinder::CYLINDER_COLLIDER_DESC tDesc;
	tDesc.fRadius = m_fMinRange;
	tDesc.vCenter = { 0.f,1.5f,0.f };
	tDesc.fHeight = 3.f;
	m_pCollider = static_cast<CCollider_Cylinder*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Cylinder::m_szProtoTag, &tDesc));

	//CastEffect2
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_wildfire_cast_02.effmodel");
	m_pCastEffect2 = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect2->Set_Active(false);
	Add_Child(m_pCastEffect2);


	return S_OK;
}

void CBullet_WildFire::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bExpanding)
	{
		if (m_fCurrentRange >= m_fMaxRange)
		{
			m_bExpanding = false;
			Set_Active(false);
			return;
		}
		static_cast<CCollider_Cylinder*>(m_pCollider)->Set_Radius(m_fCurrentRange);

		list<CGameObject*> listTarget;
		SearchTarget(&listTarget, LAYER_MONSTER);
		for (auto& pTarget : listTarget)
		{
			if (m_setHitObject.find(pTarget) != m_setHitObject.end())
				continue;
			m_setHitObject.insert(pTarget);
			m_pGameInstance->Push_Event(CHitEvent::Create(m_pShooter, pTarget, (_int)m_fDamage, m_szHitEffectTag));
		}
		m_fCurrentRange += m_fExpandSpeed * fTimeDelta;
	}
}

void CBullet_WildFire::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBullet_WildFire::Render()
{
	return __super::Render();
}


_bool CBullet_WildFire::Check_Collision(CGameObject* pOther)
{
	return static_cast<CCollider_Cylinder*>(m_pCollider)->Contains(pOther->Get_TransformPosition());
}

void CBullet_WildFire::Launch(_float fDamage, _fvector vPosition)
{
	__super::Launch(fDamage, vPosition);

	m_pCastEffect2->Start_Animation();
	m_pCastEffect2->Set_Active(true);
	m_fCurrentRange = m_fMinRange;
	m_bExpanding = true;
	m_setHitObject.clear();
	m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());
}



CBullet_WildFire* CBullet_WildFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_WildFire* pInstance = new CBullet_WildFire(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_WildFire");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_WildFire::Clone(void* pArg)
{
	CBullet_WildFire* pInstance = new CBullet_WildFire(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_WildFire");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_WildFire::Free()
{
	__super::Free();
	Safe_Release(m_pCastEffect2);
}

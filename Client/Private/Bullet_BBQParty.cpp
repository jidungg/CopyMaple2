#include "stdafx.h"
#include "Bullet_BBQParty.h"
#include "GameInstance.h"
#include "Collider_Cylinder.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModel.h"
#include "Sound.h"

CBullet_BBQParty::CBullet_BBQParty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CBullet_BBQParty::CBullet_BBQParty(const CBullet& Prototype)
	: CBullet(Prototype)
{
}


HRESULT CBullet_BBQParty::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CCollider_Cylinder::CYLINDER_COLLIDER_DESC tDesc;
	tDesc.fRadius = 3.f;
	tDesc.vCenter = { 0.f,3.f,0.f };
	tDesc.fHeight = 6.f;
	m_pCollider = static_cast<CCollider_Cylinder*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Cylinder::m_szProtoTag, &tDesc));


	//KeepEffect
	CEffModelObject::EFFECTOBJ_DESC tEffectDesc;
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_wizard_bbqparty_keep_01.effmodel");
	m_pKeepEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pKeepEffect);
	m_pKeepEffect->Register_OnAnimEndCallBack(bind(&CBullet_BBQParty::On_KeepEffectAnimEnd, this, placeholders::_1));
	m_pKeepEffect->Set_Active(false);
	m_pKeepEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, _vector{ 0,0.01,0,1 });
	m_pKeepEffect->Get_Transform()->Scaling(1.5, 1.5, 1.5);

	//SplashInvokeEffect
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_wizard_bbqparty_splashinvoke_01.effmodel");
	m_pSplashInvokeEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	m_pSplashInvokeEffect->Register_OnAnimEndCallBack(bind(&CBullet_BBQParty::On_SplashCastEffectAnimEnd, this, placeholders::_1));
	Add_Child(m_pSplashInvokeEffect);
	m_pSplashInvokeEffect->Set_Active(false);


	//SplashCastEffect
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_wizard_bbqparty_splashcast_02.effmodel");
	m_pSplashCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pSplashCastEffect);
	m_pSplashCastEffect->Set_Active(false);
	m_pSplashCastEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, _vector{ 0,0.01,0,1 });


	//SplashEndEffect
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_wizard_bbqparty_splashend_02.effmodel");
	m_pSplashEndEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pSplashEndEffect);
	m_pSplashEndEffect->Register_OnAnimEndCallBack(bind(&CBullet_BBQParty::On_SplashEndEffectAnimEnd, this, placeholders::_1));
	m_pSplashEndEffect->Set_Active(false);
	m_pSplashEndEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, _vector{ 0,0.01,0,1 });

	return S_OK;
}

void CBullet_BBQParty::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_fDurationAcc >= m_fMaxDuration)
	{
		Set_Active(false);
		m_fDurationAcc = 0.f;
		m_fDamgTimeAcc = 0.f;
		m_pKeepEffect->Set_Active(false);
		m_pSplashInvokeEffect->Set_Active(false);
		m_pSplashCastEffect->Set_Active(false);
		m_pSplashEndEffect->Set_Active(false);
		return;
	}
	m_fDurationAcc += fTimeDelta;
	m_fDamgTimeAcc += fTimeDelta;
}

void CBullet_BBQParty::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);

	if (m_fDamgTimeAcc >= m_fDamgInterval)
	{

		list<CGameObject*> listTarget;
		SearchTarget(&listTarget,LAYER_MONSTER);
		if (listTarget.size() > 0)
		{
			m_fDamgTimeAcc = 0;
			m_iSplashCount++;

			if (m_iMaxSplashCount < m_iSplashCount)
			{
				Set_Active(false);
				m_fDurationAcc = 0.f;
				m_fDamgTimeAcc = 0.f;
				m_pKeepEffect->Set_Active(false);
				m_pSplashInvokeEffect->Set_Active(false);
				m_pSplashCastEffect->Set_Active(false);
				m_pSplashEndEffect->Set_Active(false);
			}
			else
			{
				m_pSplashInvokeEffect->Start_Animation(0,false, 1);
				for (auto& pTarget : listTarget)
				{
					_bool bCrit;
					_float fDamage = m_pSkill->Calc_Damg(bCrit);
					m_pGameInstance->Push_Event(CDamgEvent::Create(m_pShooter, pTarget, (_int)fDamage, bCrit,true, EFF_MODEL_ID::HIT_KINDLING));
				}
				CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_LOADING, TEXT("Skill_Wizard_BBQParty_SplashInvoke_02.wav"));
				pSouind->SetVolume(100);
			}
		}



	}
}

HRESULT CBullet_BBQParty::Render()
{
	return __super::Render();

}

_bool CBullet_BBQParty::Check_Collision(CGameObject* pOther)
{
	
	return static_cast<CCollider_Cylinder*>( m_pCollider)->Contains(pOther->Get_TransformPosition());
}

void CBullet_BBQParty::Launch(CSkill* pSkill, _fvector vPosition)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pSkill = pSkill;
	m_pKeepEffect->Start_Animation(0, true, m_fMaxDuration);
	m_pKeepEffect->Set_Active(true);
	m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());
	m_fDurationAcc = 0;
	m_fDamgTimeAcc = m_fDamgInterval;
	m_iSplashCount = 0;
	Set_Active(true);



}

void CBullet_BBQParty::Launch(CSkill* pSkill, CGameObject* pTarget)
{
	Launch(pSkill, pTarget->Get_TransformPosition());

}

void CBullet_BBQParty::On_KeepEffectAnimEnd(CEffModel* pModel)
{

}

void CBullet_BBQParty::On_SplashCastEffectAnimEnd(CEffModel* pModel)
{
}

void CBullet_BBQParty::On_SplashEndEffectAnimEnd(CEffModel* pModel)
{
}

CBullet_BBQParty* CBullet_BBQParty::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_BBQParty* pInstance = new CBullet_BBQParty(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_BBQParty");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_BBQParty::Clone(void* pArg)
{
	CBullet_BBQParty* pInstance = new CBullet_BBQParty(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_BBQParty");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_BBQParty::Free()
{
	__super::Free();
}

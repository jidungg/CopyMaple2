#include "stdafx.h"
#include "Bullet_BayarStoneSpike.h"
#include "GameInstance.h"
#include "Collider_AABB.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModel.h"
#include "Sound.h"

constexpr _int SPIKE_COUNT = 5;
_float spikeOffsetsX[SPIKE_COUNT] = { 0.f,  1.f,  0.f, -1.f  ,  0.f };
_float spikeOffsetsY[SPIKE_COUNT] = { 0.f,  0.f,  1.f,  0.f  , -1.f };

CBullet_BayarStoneSpike::CBullet_BayarStoneSpike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}
CBullet_BayarStoneSpike::CBullet_BayarStoneSpike(const CBullet& Prototype)
	: CBullet(Prototype)
{
}
HRESULT CBullet_BayarStoneSpike::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CCollider_AABB::AABB_COLLIDER_DESC tDesc;
	tDesc.vExtentes = { 0.5f,0.5f,0.5f };
	tDesc.vCenter = { 0.f,0.5f,0.f };
	m_pCollider = static_cast<CCollider_AABB*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_AABB::m_szProtoTag, &tDesc));

	//PrecursorEffec
	CEffModelObject::EFFECTOBJ_DESC tEffectDesc;
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_sandstonegiant_offencekeep_cube.effmodel");
	m_pPrecursorEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pPrecursorEffect);
	m_pPrecursorEffect->Set_Active(false);

	//RockCUbeEffect
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_sandstonegiant_rock_cube_b.effmodel");
	m_pRockCubeEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pRockCubeEffect);
	m_pRockCubeEffect->Set_Active(false);
	m_pRockCubeEffect->Get_Transform()->Scaling(1.2, 1.2, 1.2);
	return S_OK;
}
void CBullet_BayarStoneSpike::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);


}
void CBullet_BayarStoneSpike::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
	if (m_fTimeAcc >= m_fWaitTime)
	{
		if (m_bSpiked)
		{
			if (false == m_pRockCubeEffect->Is_Active())
			{
				//cout << "Spike TIme : " << m_fTmp << " / Spike End Time : " << m_fTimeAcc << endl;
				Set_Active(false);
				return;
			}
			list<CGameObject*> listTarget;
			XMMATRIX matWorld = XMLoadFloat4x4( &m_WorldMatrix);
			for (_uint i = 0; i < SPIKE_COUNT; i++)
			{
				_vector vPos = m_vCenterPos+ _vector{ spikeOffsetsX[i], 0, spikeOffsetsY[i], 1 };
				matWorld.r[3] = XMVectorSetW(vPos,1);
				m_pCollider->Update(matWorld);
				m_pCollider->Render();
				SearchTarget(&listTarget, LAYER_PLAYER);
			}
			for (auto& pTarget : listTarget)
			{
				if (m_setHitObject.find(pTarget) != m_setHitObject.end())
					continue;
				m_setHitObject.insert(pTarget);
				_bool bCrit;
				_float fDamage = m_pSkill->Calc_Damg(bCrit);
				m_pGameInstance->Push_Event(CDamgEvent::Create(m_pShooter, pTarget, (_int)fDamage,bCrit,false, m_eHitEffect));
			}


		}
		else
		{
			m_fTmp = m_fTimeAcc;
			m_pRockCubeEffect->Set_Active(true);
			m_pRockCubeEffect->Start_Animation();
			m_bSpiked = true;
			CSound* pSound = CGameInstance::GetInstance()->Start_EffectPlay_Random(LEVEL_BAYARPEAK, TEXT("Boss_SandstoneGiant_Skill_Attack_02_G_Stone_0%d.wav"), 1, 8);
			pSound->SetVolume(100);
		}
	}
	m_fTimeAcc += fTimeDelta;
}
HRESULT CBullet_BayarStoneSpike::Render()
{
	for (_uint i = 0; i < SPIKE_COUNT; i++)
	{
		_vector vPos = _vector{ spikeOffsetsX[i], 0.1f, spikeOffsetsY[i], 1.f };
		m_pPrecursorEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos);
		m_pRockCubeEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos);
		m_pRockCubeEffect->Compute_Matrix();
		m_pPrecursorEffect->Compute_Matrix();
		__super::Render();
	}
	return S_OK;
}

void CBullet_BayarStoneSpike::Launch(CSkill* pSkill, _vector vPosition)
{
	__super::Launch(pSkill, vPosition);
	m_pPrecursorEffect->Set_Active(true);
	m_pPrecursorEffect->Start_Animation();
	m_pRockCubeEffect->Set_Active(false);
	m_fTimeAcc = 0.f;
	m_bSpiked = false;
	m_vCenterPos = vPosition;
	m_setHitObject.clear();
}
CBullet_BayarStoneSpike* CBullet_BayarStoneSpike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_BayarStoneSpike* pInstance = new CBullet_BayarStoneSpike(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_BayarStoneSpike");
		Safe_Release(pInstance);
	}
	return pInstance;
}
CGameObject* CBullet_BayarStoneSpike::Clone(void* pArg)
{
	CBullet_BayarStoneSpike* pInstance = new CBullet_BayarStoneSpike(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_BayarStoneSpike");
		Safe_Release(pInstance);
	}
	return pInstance;
}
void CBullet_BayarStoneSpike::Free()
{
	__super::Free();
}


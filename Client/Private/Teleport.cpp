#include "stdafx.h"
#include "Teleport.h"
#include "Character.h"
#include "EffModelObject.h"
#include "GameInstance.h"
#include "Sound.h"

CTeleport::CTeleport()
	:CSkill()
{
}

HRESULT CTeleport::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;
	m_bNeedWeapon = true;
	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_teleport_cast_01_a.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);
//m_pUser->Add_Child(m_pCastEffect);

	return S_OK;
}

void CTeleport::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
		m_pCastEffect->Update(fTimeDelta);
}

void CTeleport::Late_Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}

}

void CTeleport::Fire()
{
	m_pUser->Move_Forward(m_pSkillDesc->vecData[m_pSkillDesc->iLevel - 1]);
	m_pCastEffect->Start_Animation();
	m_pCastEffect->Set_Active(true);
	_vector vPos = m_pUser->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	vPos += _vector{ 0,0.01,0,0 };
	m_pCastEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos);

	CSound* pSouind = m_pGameInstance->Start_EffectPlay(LEVEL_LOADING, L"Skill_Wizard_Teleport_Cast_01.wav");
	pSouind->SetVolume(100);
}

void CTeleport::On_SkillUsed()
{
}

void CTeleport::On_CastingEnd()
{
}
CTeleport* CTeleport::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CTeleport* pInstance = new CTeleport;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CTeleport Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTeleport::Free()
{
	__super::Free();
	Safe_Release(m_pCastEffect);
}

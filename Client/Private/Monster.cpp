#include "stdafx.h"
#include "Monster.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "ModelObject.h"
#include "MonsterDataBase.h"
#include "MonsterAnimStateMachine.h"
#include "StateMachine.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CCharacter(Prototype)
{
}


HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
	m_pMonData = MONSTERDB->Get_Data(pDesc->eMonID);
	pDesc->fBodyCollisionOffset = { 0,0.2f,0 };
	pDesc->fBodyCollisionRadius = 0.2f;
	pDesc->fRotationPerSec = XMConvertToRadians(90.f);
	pDesc->fSpeedPerSec = 2.0f;
	pDesc->iColliderCount = COLLIDER_LAST;
	pDesc->iBodyColliderIndex = COLLIDER_BODY;
	m_tStat = m_pMonData->tStat;
	m_fDetectionRange = m_pMonData->fDetectionRange;
	m_fChaseRange = m_pMonData->fChaseRange;
	m_mapAnimIdx = m_pMonData->mapAnimIdx;
	m_vHomePos = pDesc->vHomePos;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Parts(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_AnimStateMachine()))
		return E_FAIL;

	Set_Position(m_vHomePos);
	return S_OK;
}

HRESULT CMonster::Ready_Components(MONSTER_DESC* pDesc)
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	tDesc.vCenter = { 0,0,0 };
	tDesc.fRadius = m_fDetectionRange;
	m_vecCollider[COLLIDER_DETECT] = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));
	Add_Component(m_vecCollider[COLLIDER_DETECT], L"Com_Collider_Detection");

	return S_OK;
}

HRESULT CMonster::Ready_Parts(MONSTER_DESC* pDesc)
{
	//Body
	CModelObject::MODELOBJ_DESC tModelDesc = {};
	tModelDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tModelDesc.fSpeedPerSec = 5.f;
	tModelDesc.eModelProtoLevelID = LEVEL_GAMEPLAY;
	strcpy_s(tModelDesc.strModelProtoName, m_pMonData->strModelTag);
	m_pBody = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	Add_Child(m_pBody);
	return S_OK;
}
HRESULT CMonster::Ready_AnimStateMachine()
{
	m_pBody->Set_AnimationLoop(m_mapAnimIdx[M_AS_WALK].front(), true);
	m_pBody->Set_AnimationLoop(m_mapAnimIdx[M_AS_RUN].front(), true);
	m_pBody->Set_AnimationLoop(m_mapAnimIdx[M_AS_IDLE].front(), true);
	m_pBody->Set_AnimationLoop(m_mapAnimIdx[M_AS_ATTACK_IDLE].front(), true);
	for (auto& i : m_mapAnimIdx[M_AS_ATTACK])
		m_pBody->Set_AnimPostDelayPercent(i, 1);
	for (auto& i : m_mapAnimIdx[M_AS_STUN])
		m_pBody->Set_AnimPostDelayPercent(i, 1);
	for (auto& i : m_mapAnimIdx[M_AS_DEAD])
		m_pBody->Set_AnimPostDelayPercent(i, 1);


	CMonsterAnimStateMachine::MONSTER_ANIM_STATEMACHINE_DESC tDesc;
	tDesc.mapAnimIdx = &m_mapAnimIdx;
	tDesc.bDetected = &m_bDetected;
	tDesc.bAttack = &m_bAttack;
	tDesc.bMove = &m_bMove;
	tDesc.bStun = &m_bStun;
	tDesc.iHp = &m_tStat.iHP;
	m_pAnimStateMachine = static_cast<CStateMachine*>( m_pGameInstance->Clone_Proto_Component_Stock(CMonsterAnimStateMachine::m_szProtoTag, &tDesc));
	Add_Component(m_pAnimStateMachine, L"Com_AnimStateMachine");
	m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CMonster::On_StateChange, this, placeholders::_1));
	m_pAnimStateMachine->Register_OnSubStateChangeCallBack(bind(&CMonster::On_SubStateChange, this, placeholders::_1));

	m_pBody->Set_Animation((_uint)m_mapAnimIdx[M_AS_REGEN].front());
	return S_OK;
}

void CMonster::Use_Skill(CSkill* pSkill)
{
}

void CMonster::Priority_Update(_float fTimeDelta)
{

	_vector vMyPos = Get_Position();
	_vector vDestination;
	_float fMoveSpeed = m_tStat.fRunSpeed;
	_float fHomeDistance;
	XMStoreFloat(&fHomeDistance, XMVector3Length(vMyPos - m_vHomePos));
	if (m_bBackToHome)
	{
		if (fHomeDistance <= 0.1f)
		{
			m_bBackToHome = false;
			m_bDetected = false;
			m_pTarget = nullptr;
			m_fTargetDistance = FLT_MAX;
			return;
		}
		fMoveSpeed = m_tStat.fRunSpeed *2;
		vDestination = m_vHomePos;
	}
	else if (fHomeDistance > m_fHomeRange)
	{
		m_pTarget = nullptr;
		m_bDetected = false;
		m_bBackToHome = true;
		m_fTargetDistance = FLT_MAX;
		vDestination = m_vHomePos;
	}
	else if(nullptr == m_pTarget || false == m_pTarget->Is_Active() || m_pTarget->Is_Dead())
	{
		m_pTarget = nullptr;
		m_bDetected = false;
		m_fTargetDistance = FLT_MAX;
		return;
	}
	else
		vDestination = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_POSITION);


	m_vMoveDirectionXZ = vDestination - vMyPos;
	m_vMoveDirectionXZ = XMVectorSetY(m_vMoveDirectionXZ, 0);
	XMStoreFloat(&m_fTargetDistance, XMVector3Length(m_vMoveDirectionXZ));
	m_vMoveDirectionXZ = XMVector3Normalize(m_vMoveDirectionXZ);
	m_vLookDirectionXZ = m_vMoveDirectionXZ;
	m_vMoveDirectionXZ *= fMoveSpeed * fTimeDelta;

	if (m_bBackToHome)
		return;
	//공격 쿨이 다 됐으면 공격 범위 안까지 추적
	//공격 쿨이 안됐으면
	//추적 상태일 경우 공격범위 안까지 추적
	//추적 상태가 아닐 경우 추적범위 밖에 있지 않으면 안추적
	if (Is_AttackCoolReady())
	{

		if (m_fTargetDistance <= m_tStat.fAttackRange)
		{
			m_bAttack = true;
			m_bChase = false;
			m_fAttackTimeAcc = 0.f;
			m_pAnimStateMachine->Trigger_ConditionVariable(CMonsterAnimStateMachine::AC_ATTACKTRIGGER);
			m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
		}

	}
	//공격 쿨임 : 추적상태면 쫒고 아니면 안쫒기
	else
	{
		// 추적상태가 아니다 -> 공격범위에 들어온 뒤로 추적범위 밖으로 나가지 않았음.
		if (false ==  m_bChase) 
		{
			if(m_fTargetDistance > m_fChaseRange)//추적 범위 밖으로 나가면 추적
				m_bChase = true;
		}
		else //추적 상태다 -> 추적 범위 밖으로 나간 뒤 공격 범위 안으로 들어오지 않았음.
		{
			if (m_fTargetDistance <= m_tStat.fAttackRange)//공격 범위 안으로 들어올 때까지 추적
				m_bChase = false;
		}
		if (false == m_bChase)
			m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
	}
	
	if (m_bAttack)
	{
		m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
		m_vLookDirectionXZ = XMVectorZero();
	}


	__super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	for (auto& pCollider : m_vecCollider)
	{
		pCollider->Update(XMLoadFloat4x4(&m_WorldMatrix));
	}
}

_bool CMonster::Check_Collision(CGameObject* pOther)
{


	LAYERID eLayerID = (LAYERID)pOther->Get_LayerID();
	switch (eLayerID)
	{
	case Client::LAYER_PLAYER:
	{
		_vector vPos = pOther->Get_Transform()->Get_State(CTransform::STATE_POSITION);


		if (m_vecCollider[COLLIDER_DETECT]->Contains(vPos))
		{
			m_pTarget = pOther;
			m_bDetected = true;
			return true;
		}




	}
	case Client::LAYER_TERRAIN:
	case Client::LAYER_MONSTER:
	case Client::LAYER_INTERACTION:
	case Client::LAYER_UI:
	case Client::LAYER_CAMERA:
	case Client::LAYER_LAST:
	default:
		break;
	}
	return __super::Check_Collision(pOther);
}

void CMonster::Late_Update(_float fTimeDelta)
{

	if (m_pBody->Is_AnimEnd())
	{
		if (m_pBody->Get_AnimIndex() == m_mapAnimIdx[M_AS_ATTACK].front())
		{
			m_bAttack = false;
		}
		m_pAnimStateMachine->Trigger_ConditionVariable(CMonsterAnimStateMachine::AC_ANIMENDTRIGGER);
	}

	if (Is_AttackCoolReady())
		m_fAttackTimeAcc = m_tStat.fAttackInterval;
	else
		m_fAttackTimeAcc += fTimeDelta;


	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMonster::Render()
{
	cout << "LookDirection : " << m_vLookDirectionXZ.m128_f32[0] << endl;
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active() && child->Is_Dead() == false)
			child->Render();
	}
	for (auto& pCollider : m_vecCollider)
		pCollider->Render();

	return __super::Render();
}

void CMonster::On_StateChange(_uint iState)
{
	cout << "Monster State Changed : " << iState << endl;
}

void CMonster::On_SubStateChange(_uint iSubState)
{
	cout << "Monster SubState Changed : " << iSubState << endl;
	m_pBody->Switch_Animation(iSubState);
}


CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "Bayar.h"
#include "ModelObject.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "SkillManager.h"
#include "Collider_Sphere.h"

CBayar::CBayar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CBayar::CBayar(const CBayar& Prototype)
	: CCharacter(Prototype)
{
}

HRESULT CBayar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBayar::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	if (FAILED(Ready_Components()))
		return E_FAIL;
    if (FAILED(Ready_Parts()))
        return E_FAIL;
    if (FAILED(Ready_AnimStateMachine()))
        return E_FAIL;
    if (FAILED(Ready_Skill()))
        return E_FAIL;

	m_pTransformCom->Scaling(2.f, 2.f, 2.f);
    return S_OK;
}

void CBayar::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CBayar::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CBayar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBayar::Render()
{
    for (auto& child : m_pChilds)
    {
        if (child->Is_Active() && child->Is_Dead() == false)
            child->Render();
    }
    m_pDetectionCollider->Render();
    return S_OK;
}

HRESULT CBayar::Ready_Components()
{
	//Collider
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc = {};
	tDesc.fRadius = m_fDetectionRange;
    auto vScale =  m_pTransformCom->Compute_Scaled();
	tDesc.vCenter = { 0.f,vScale.y/2, 0.f };
	if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Sphere::m_szProtoTag, TEXT("Com_Collider"), (CComponent**)&m_pDetectionCollider, &tDesc)))
		return E_FAIL;
    return S_OK;
}

HRESULT CBayar::Ready_Parts()
{
    //Body
    CModelObject::MODELOBJ_DESC tModelDesc = {};
	tModelDesc.fRotationPerSec = XMConvertToRadians(90.f);
    tModelDesc.fSpeedPerSec = 5.f;
    tModelDesc.eModelProtoLevelID = LEVEL_LOADING;
    strcpy_s(tModelDesc.strModelProtoName, "Bayar.model");
    m_pBody = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
    Add_Child(m_pBody);


    return S_OK;
}

HRESULT CBayar::Ready_AnimStateMachine()
{
    m_pBody->Set_Animation((_uint)ANIM_STATE::REGEN);
    return S_OK;
}

HRESULT CBayar::Ready_FaceStateMachine()
{
    return S_OK;
}

HRESULT CBayar::Ready_Skill()
{
    return S_OK;
}

void CBayar::On_StateChange(_uint iState)
{
}

void CBayar::On_SubStateChange(_uint iSubState)
{
}

void CBayar::On_FaceStateChange(_uint iState)
{
}

void CBayar::Use_Skill(CSkill* pSkill)
{
}

CBayar* CBayar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBayar* pInstance = new CBayar(pDevice, pContext);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

CGameObject* CBayar::Clone(void* pArg)
{
    CBayar* pInstance = new CBayar(*this);
    if (FAILED(pInstance->Initialize(pArg)))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CBayar::Free()
{
    __super::Free();
}

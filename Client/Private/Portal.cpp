#include "stdafx.h"
#include "Portal.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "LevelChangeEvent.h"
#include "Collider_AABB.h"
#include "EffModel.h"

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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	PORTAL_DESC* pDesc = static_cast<PORTAL_DESC*>(pArg);
	m_eLevelID = pDesc->eLevelID;
	//Model
	m_pEffModelCom = static_cast<CEffModel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, LEVEL_LOADING, TEXT("eff_com_portal_a.effmodel"), nullptr));
	if (FAILED(Add_Component(m_pEffModelCom, TEXT("Com_Model"))))
		return E_FAIL;
	m_pEffModelCom->Set_Active(true);
	m_pEffModelCom->Set_Loop(true);
	m_pEffModelCom->Start_Animation();

	//Com_Shader 
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxEffectMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
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
	m_pEffModelCom->Update_Animation(fTimeDelta, XMLoadFloat4x4( &m_WorldMatrix));
}

void CPortal::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
	m_vecCollider[0]->Update(XMLoadFloat4x4(&m_WorldMatrix));
}

_bool CPortal::Check_Collision(CGameObject* pOther)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
	return m_vecCollider[0]->Intersects(pOther->Get_Collider(0));
}

HRESULT CPortal::Render()
{
	__super::Render();
	if (FAILED(Bind_ShaderResources(m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pEffModelCom->Render(m_pShaderCom)))
		return E_FAIL;
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

HRESULT CPortal::Bind_ShaderResources(CShader* pShader)
{
	if (FAILED(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(pShader->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(pShader->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);

	if (FAILED(pShader->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
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

void CPortal::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pEffModelCom);
}





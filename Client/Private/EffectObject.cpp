#include "stdafx.h"
#include "EffectObject.h"
#include "EffModel.h"
#include "Shader.h"
#include "GameInstance.h"

CEffectObject::CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CEffectObject::CEffectObject(const CEffectObject& Prototype)
	:CGameObject(Prototype),
	m_pModelCom{ Prototype.m_pModelCom },
	m_pShaderCom{ Prototype.m_pShaderCom }
{
	Safe_AddRef(m_pModelCom);
	Safe_AddRef(m_pShaderCom);
}

HRESULT CEffectObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	return S_OK;
}

void CEffectObject::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

HRESULT CEffectObject::Render()
{
	if (FAILED(Bind_ShaderResources(m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;
	return S_OK;
}

void CEffectObject::Play_Animation()
{

	m_pModelCom->Play_Animation();
}

HRESULT CEffectObject::Ready_Components(void* pArg)
{
	EFFECTOBJ_DESC* pDesc = (EFFECTOBJ_DESC*)pArg;
	/* Com_VIBuffer */
	string tmp = pDesc->strModelProtoName;
	wstring wtmp = wstring(tmp.begin(), tmp.end());
	m_pModelCom = static_cast<CEffModel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, pDesc->eModelProtoLevelID, wtmp, nullptr));
	if (FAILED(Add_Component(m_pModelCom, TEXT("Com_Model"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxEffectMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CEffectObject::Bind_ShaderResources(CShader* pShader)
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


CEffectObject* CEffectObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffectObject* pInstance = new CEffectObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffectObject::Clone(void* pArg)
{
	CEffectObject* pInstance = new CEffectObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectObject::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

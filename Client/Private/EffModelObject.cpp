#include "stdafx.h"
#include "EffModelObject.h"
#include "EffModel.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Character.h"

CEffModelObject::CEffModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CEffModelObject::CEffModelObject(const CEffModelObject& Prototype)
	:CGameObject(Prototype),
	m_pModelCom{ Prototype.m_pModelCom },
	m_pShaderCom{ Prototype.m_pShaderCom }
{
	Safe_AddRef(m_pModelCom);
	Safe_AddRef(m_pShaderCom);
}

HRESULT CEffModelObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffModelObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	EFFECTOBJ_DESC* pDesc = (EFFECTOBJ_DESC*)pArg;
	m_bAttachToBone = pDesc->bAttachToBone;
	if (m_bAttachToBone)
	{
		m_pParentMatrix = pDesc->pAttachObject->Get_Transform()->Get_WorldFloat4x4_Ptr();
		m_pSocketMatrix = static_cast<CCharacter*>(pDesc->pAttachObject)->Get_BoneMatrix(pDesc->szBoneName);
	}
	return S_OK;
}

void CEffModelObject::Update(_float fTimeDelta)
{
	if (m_fDuration > 0 && m_fTimeAcc >= m_fDuration)
	{
		m_pModelCom->Stop_Animation();
		m_fTimeAcc = 0;
		return;
	}
	if (m_fStartDelay <= m_fTimeAcc)
	{
		__super::Update(fTimeDelta);
		m_pModelCom->Update_Animation(fTimeDelta);
	}
	m_fTimeAcc += fTimeDelta;
}

HRESULT CEffModelObject::Render()
{
	if (FAILED(Bind_ShaderResources(m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;
	return S_OK;
}



void CEffModelObject::Compute_Matrix()
{
	if (m_bAttachToBone)
	{
		assert(nullptr != m_pParentMatrix);
		assert(nullptr != m_pSocketMatrix);
		_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		for (size_t i = 0; i < 3; i++)
			SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);

		XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));
	}
	else
		__super::Compute_Matrix();
}

void CEffModelObject::Start_Animation(_float fStartDealy, _bool bLoop, _float fDuration)
{
	m_fStartDelay = fStartDealy;
	m_fDuration = fDuration;
	m_pModelCom->Set_Loop(bLoop);
	On_EffectAnimStart(m_pModelCom);
	m_pModelCom->Start_Animation();
}



void CEffModelObject::Register_OnAnimEndCallBack(const function<void(CEffModel*)>& fCallback)
{
	m_pModelCom->Register_OnAnimEndCallBack(fCallback);
}

void CEffModelObject::Register_AnimEvent(ANIM_EVENT tAnimEvent)
{
	m_pModelCom->Register_AnimEvent(tAnimEvent);
}

HRESULT CEffModelObject::Ready_Components(void* pArg)
{
	EFFECTOBJ_DESC* pDesc = (EFFECTOBJ_DESC*)pArg;
	/* Com_VIBuffer */
	string tmp = pDesc->strModelProtoName;
	wstring wtmp = wstring(tmp.begin(), tmp.end());
	m_pModelCom = static_cast<CEffModel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, pDesc->eModelProtoLevelID, wtmp, nullptr));
	if (FAILED(Add_Component(m_pModelCom, TEXT("Com_Model"))))
		return E_FAIL;
	m_pModelCom->Register_OnAnimEndCallBack(bind(&CEffModelObject::On_EffectAnimEnd, this, placeholders::_1));
	/* Com_Shader */
	if (FAILED(Add_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxEffectMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CEffModelObject::Bind_ShaderResources(CShader* pShader)
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


CEffModelObject* CEffModelObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffModelObject* pInstance = new CEffModelObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffModelObject::Clone(void* pArg)
{
	CEffModelObject* pInstance = new CEffModelObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffModelObject::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

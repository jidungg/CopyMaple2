#include "stdafx.h"
#include "ModelObject.h"

#include "GameInstance.h"
#include "Mesh.h"
#include "Model.h"
#include "MeshCollider.h"
#include "Client_Utility.h"

CModelObject::CModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CModelObject::CModelObject(const CModelObject& Prototype)
	: CGameObject(Prototype),
    m_pModelCom{ Prototype.m_pModelCom },
    m_pShaderCom{ Prototype.m_pShaderCom }
{
    Safe_AddRef(m_pModelCom);
    Safe_AddRef(m_pShaderCom);
}

HRESULT CModelObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CModelObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CModelObject::Update(_float fTimeDelta)
{
	if (m_eModelType == CModel::TYPE::TYPE_ANIM)
        m_bAnimationEnd =m_pModelCom->Play_Animation(fTimeDelta);
	__super::Update(fTimeDelta);
}

bool CModelObject::Is_AnimPostDelayEnd()
{
    return m_pModelCom->Is_AnimChangeable(); 
}

HRESULT CModelObject::Ready_Components(void* pArg)
{
    MODELOBJ_DESC* pDesc = (MODELOBJ_DESC*)pArg;
    /* Com_VIBuffer */
    string tmp = pDesc->strModelProtoName;
    wstring wtmp = wstring(tmp.begin(), tmp.end());
	CModel::ModelDesc modelDesc;
    modelDesc.pMimicTarget =  pDesc->pMimicTarget;
    m_pModelCom = static_cast<CModel*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, pDesc->eModelProtoLevelID,wtmp,&modelDesc));
    if (FAILED(Add_Component(m_pModelCom, TEXT("Com_Model"))))
        return E_FAIL;

	m_eModelType = m_pModelCom->Get_Type();

    /* Com_Shader */
    wtmp = m_eModelType == CModel::TYPE_NONANIM ? TEXT("Prototype_Component_Shader_VtxMesh") : TEXT("Prototype_Component_Shader_VtxAnimMesh");

    if (FAILED(Add_Component(LEVEL_LOADING, wtmp,
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;
    //m_pTransformCom->LookToward(Get_Direction_Vector(pDesc->direction));
    return S_OK;
}


HRESULT CModelObject::Render()
{
    if (FAILED(Bind_ShaderResources(m_pShaderCom)))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEXTURE_TYPE::DIFFUSE, 0)))
            return E_FAIL;
		if (CModel::TYPE::TYPE_NONANIM != m_eModelType)
            if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
                return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }


    for (auto& child : m_pChilds)
        child->Render();

    return S_OK;
}

const _float4x4* CModelObject::Get_BoneMatrix(const _char* pBoneName) const
{
    return m_pModelCom->Get_BoneMatrix(pBoneName);
}

_uint CModelObject::Get_AnimIndex()
{
    return m_pModelCom->Get_AnimIndex();
}

void CModelObject::Set_AnimationLoop(_uint iIdx, _bool bIsLoop)
{
	m_pModelCom->Set_AnimationLoop(iIdx, bIsLoop);
}

void CModelObject::Set_Animation(_uint iIdx)
{
	m_pModelCom->Set_Animation(iIdx);
}

void CModelObject::Set_AnimPostDelayPercent(_uint iIdx, _float fPercent)
{
	m_pModelCom->Set_AnimPostDelayPercent(iIdx, fPercent);
}

void CModelObject::Switch_Animation(_uint iIdx)
{
	m_pModelCom->Switch_Animation(iIdx);
}

HRESULT CModelObject::Replace_Model(MODELOBJ_DESC* pDesc)
{
    Remove_Component(m_pModelCom);
    /* Com_VIBuffer */
    string tmp = pDesc->strModelProtoName;
    wstring wtmp = wstring(tmp.begin(), tmp.end());
    CModel::ModelDesc modelDesc;
    modelDesc.pMimicTarget = pDesc->pMimicTarget;
    m_pModelCom = static_cast<CModel*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, pDesc->eModelProtoLevelID, wtmp, &modelDesc));
    if (FAILED(Add_Component(m_pModelCom, TEXT("Com_Model"))))
        return E_FAIL;

}



HRESULT CModelObject::Bind_ShaderResources(CShader* pShader)
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(pShader, "g_WorldMatrix")))
        return E_FAIL;
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

CModelObject* CModelObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

    CModelObject* pInstance = new CModelObject(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CModelObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CModelObject::Clone(void* pArg)
{
    CModelObject* pInstance = new CModelObject(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CModelObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CModelObject::Free()
{
    __super::Free();
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}

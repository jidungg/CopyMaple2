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
    m_pTransformCom->TurnToward(Get_Direction_Vector(m_eDestDirection), fTimeDelta);
	__super::Update(fTimeDelta);
}

HRESULT CModelObject::Ready_Components(void* pArg)
{
    MODELOBJ_DESC* pDesc = (MODELOBJ_DESC*)pArg;
    /* Com_Shader */
    if (FAILED(Add_Component(pDesc->eShaderProtoLevelID, pDesc->wstrShaderProtoName,
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(Add_Component(pDesc->eModelProtoLevelID, pDesc->wstrModelProtoName,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}


HRESULT CModelObject::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
            return E_FAIL;
        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }


    for (auto& child : m_pChilds)
        child->Render();

    return S_OK;
}

void CModelObject::ReplaceModel(CModel* pModel)
{
	Safe_Release(m_pModelCom);
	m_pModelCom = pModel;
	Safe_AddRef(m_pModelCom);
}

void CModelObject::Rotate()
{
	m_eDestDirection = (DIRECTION)(((_uint)m_eDestDirection + 1) % (_uint)DIRECTION::XMZP);

}


HRESULT CModelObject::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);


    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
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

#include "stdafx.h"
#include "MimicBoneModelObject.h"
#include "MimicBoneModel.h"
#include "Shader.h"

CMimicBoneModelObject::CMimicBoneModelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModelObject(pDevice, pContext)
{
}

CMimicBoneModelObject::CMimicBoneModelObject(const CMimicBoneModelObject& Prototype)
	: CModelObject(Prototype)
{
}



HRESULT CMimicBoneModelObject::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TEXTURE_TYPE::DIFFUSE, 0)))
            return E_FAIL;
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }


    for (auto& child : m_pChilds)
        child->Render();

    return S_OK;
}

HRESULT CMimicBoneModelObject::Ready_Components(void* pArg)
{
    MIMICBONEOBJECT_DESC* pDesc = (MIMICBONEOBJECT_DESC*)pArg;
    m_eModelType = pDesc->eModelType;
    /* Com_Shader */
    string tmp = pDesc->strShaderProtoName;
    wstring wtmp = wstring(tmp.begin(), tmp.end());

    if (FAILED(Add_Component(pDesc->eShaderProtoLevelID, wtmp,
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    tmp = pDesc->strModelProtoName;
    wtmp = wstring(tmp.begin(), tmp.end());
	CMimicBoneModel::MIMICBONEMODEL_DESC desc;
	desc.pTarget = pDesc->pTarget;
    if (FAILED(Add_Component(pDesc->eModelProtoLevelID, wtmp,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom),&desc)))
        return E_FAIL;


    //m_pTransformCom->LookToward(Get_Direction_Vector(pDesc->direction));
    return S_OK;
}

CMimicBoneModelObject* CMimicBoneModelObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMimicBoneModelObject* pInstance = new CMimicBoneModelObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMimicBoneModelObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMimicBoneModelObject::Clone(void* pArg)
{
	CMimicBoneModelObject* pInstance = new CMimicBoneModelObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMimicBoneModelObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMimicBoneModelObject::Free()
{
    __super::Free();
}

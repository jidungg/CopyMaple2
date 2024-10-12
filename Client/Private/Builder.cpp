#include "stdafx.h"
#include "Builder.h"

#include "GameInstance.h"
#include "Mesh.h"
#include "Model.h"
#include "MeshCollider.h"
#include "ModelObject.h"

CBuilder::CBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBuilder::CBuilder(const CBuilder& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CBuilder::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBuilder::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CModelObject::MODELOBJ_DESC pDesc;
	pDesc.eModelProtoLevelID = LEVEL_HOME;
	pDesc.wstrModelProtoName = L"Prototype_Model_ChocoDuckyBall";
	pDesc.eShaderProtoLevelID = LEVEL_LOADING;
	pDesc.wstrShaderProtoName = L"Prototype_Component_Shader_VtxMesh";
	m_pBird = static_cast<CModelObject*>( m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &pDesc));
	Add_Child(m_pBird);
	m_pBird->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(0,2,0,1));
	return S_OK;
}

void CBuilder::Update(_float fTimeDelta)
{
	
}

void CBuilder::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBuilder::Render()
{
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active() && child->Is_Dead() == false)
			child->Render();
	}
	return S_OK;
}


CBuilder* CBuilder::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBuilder* pInstance = new CBuilder(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBuilder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBuilder::Clone(void* pArg)
{
	CBuilder* pInstance = new CBuilder(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBuilder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuilder::Free()
{
	__super::Free();
}

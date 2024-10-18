#include "stdafx.h"
#include "Builder.h"

#include "GameInstance.h"
#include "Mesh.h"
#include "Model.h"
#include "MeshCollider.h"
#include "ModelObject.h"
#include "Client_Utility.h"
#include "CubeTerrain.h"
#include "TerrainObject.h"
#include "ItemDataBase.h"

CBuilder::CBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn(pDevice, pContext)
{
}

CBuilder::CBuilder(const CBuilder& Prototype)
	: CPawn(Prototype)
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
	BUILDER_DESC* pBulderDesc = static_cast<BUILDER_DESC*>(pArg);
	m_pCubeTerrain = pBulderDesc->pCubeTerrain;

	//DuckyBall
	CModelObject::MODELOBJ_DESC tModelDesc;
	tModelDesc.fRotationPerSec = 5.f;
	tModelDesc.fSpeedPerSec = 1.f;
	tModelDesc.eModelType = CModel::TYPE_ANIM;
	tModelDesc.eModelProtoLevelID = LEVEL_HOME;
	strcpy_s(tModelDesc.strModelProtoName , "Prototype_Model_ChocoDuckyBall");
	tModelDesc.eShaderProtoLevelID = LEVEL_LOADING;
	strcpy_s(tModelDesc.strShaderProtoName , "Prototype_Component_Shader_VtxAnimMesh");
	m_pBird = static_cast<CModelObject*>( m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	Add_Child(m_pBird);
	m_pBird->Set_AnimationLoop(0,true);
	if (FAILED(Ready_Preview("Prototype_Model_EmptyModel")))
		return E_FAIL;
	return S_OK;
}
HREFTYPE CBuilder::Ready_Preview(const _char* szModelTag)
{
	CTerrainObject::TERRAINOBJ_DESC tTerrObjDesc;
	tTerrObjDesc.fRotationPerSec = 5.f;
	tTerrObjDesc.fSpeedPerSec = 1.f;
	tTerrObjDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tTerrObjDesc.strModelProtoName, szModelTag);
	tTerrObjDesc.eShaderProtoLevelID = LEVEL_LOADING;
	strcpy_s(tTerrObjDesc.strShaderProtoName, "Prototype_Component_Shader_VtxMesh");
	XMStoreFloat4(&tTerrObjDesc.pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_vPreviewOffset);

	m_pPreview = static_cast<CTerrainObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CTerrainObject::m_szProtoTag, &tTerrObjDesc));
	Add_Child(m_pPreview);
	m_pPreview->Get_Transform()->Scaling(0.5f, 0.5f, 0.5f);
	return S_OK;
}

void CBuilder::Update(_float fTimeDelta)
{
	XMVECTOR vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	XMVECTOR vBirdPos =  vPos + m_vBirdOffset;
	m_pBird->Get_Transform()->Set_State(CTransform::STATE_POSITION, vBirdPos);

	XMVECTOR vPreviewPos = vPos + m_vPreviewOffset;
	m_pPreview->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPreviewPos);

	__super::Update(fTimeDelta);
}

void CBuilder::Receive_KeyInput(KEY eKey, KEY_STATE eKeyState, _float fTimeDelta)
{

	if (eKey == KEY::RIGHT && eKeyState == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_E);
	if (eKey == KEY::UP && eKeyState == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_N);
	if (eKey == KEY::DOWN && eKeyState == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_S);
	if (eKey == KEY::LEFT && eKeyState == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_W);
	if (eKey == KEY::W && eKeyState == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_U);
	if (eKey == KEY::S && eKeyState == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_D);




	if (eKey == KEY::R && eKeyState == KEY_STATE::DOWN)//ȸ��
	{
		_float4 pos;
		XMStoreFloat4(&pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_uint index = m_pCubeTerrain->PosToIndex(pos);
		CTerrainObject* pTerrObj  = m_pCubeTerrain->Get_TerrainObject(index);
		if (pTerrObj)
			pTerrObj->Rotate();
		else
			m_pPreview->Rotate();
	}
	if (eKey == KEY::E && eKeyState == KEY_STATE::DOWN) // ȸ��
	{
	}
	if (eKey == KEY::SPACE && eKeyState == KEY_STATE::DOWN) // ��ġ
	{
		CTerrainObject::TERRAINOBJ_DESC desc;
		desc.fRotationPerSec = 5.f;
		desc.fSpeedPerSec = 1.f;
		desc.eType = TERRAIN_OBJ_TYPE::BLOCK;
		strcpy_s( desc.strItemName,m_szItemName);
		ITEM_DESC* itemdesc = ITEMDB->GetItemDesc(ITEM_TYPE::BUILD, desc.strItemName);
		strcpy_s(desc.strModelProtoName, itemdesc->strModelTag);
		desc.eModelProtoLevelID = LEVEL_LOADING;
		strcpy_s(desc.strShaderProtoName, ("Prototype_Component_Shader_VtxMesh"));
		desc.eShaderProtoLevelID = LEVEL_LOADING;
		desc.direction = m_eBuildItemDir;
		desc.data = m_iBuildData;
		_float4 pos;
		XMStoreFloat4(&pos,m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		desc.index = m_pCubeTerrain->PosToIndex(pos);
		desc.pos = m_pCubeTerrain->IndexToPos(desc.index);
		 
		m_pCubeTerrain->Add_TerrainObject(desc);
	}
}

void CBuilder::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	if (false == XMVector4Equal(m_vMoveDir, XMVectorSet(0, 0, 0, 0)))
	{
		m_pTransformCom->Go_Direction(m_vMoveDir, fTimeDelta);

		m_pBird->Get_Transform()->LookToward(XMVectorSetY( m_vMoveDir,0));
	}
	m_vMoveDir = XMVectorSet(0, 0, 0, 0);
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

void CBuilder::Set_BuildItem(ITEM_DESC* tItemDesc)
{
	strcpy_s(m_szItemName, tItemDesc->strItemName);
	Remove_Child(m_pPreview);
	Safe_Release(m_pPreview);
	Ready_Preview(tItemDesc->strModelTag);
}

void CBuilder::Move_To(const _vector& vPos)
{
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

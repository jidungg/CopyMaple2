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
#include "BuildPreview.h"

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

	if (FAILED(Ready_Builder()))
		return E_FAIL;
	if (FAILED(Ready_Preview("EmptyModel.model")))
		return E_FAIL;
	return S_OK;
}
HREFTYPE CBuilder::Ready_Builder()
{
	//DuckyBall
	CModelObject::MODELOBJ_DESC tModelDesc;
	tModelDesc.fRotationPerSec = 5.f;
	tModelDesc.fSpeedPerSec = 1.f;
	//tModelDesc.eModelType = CModel::TYPE_ANIM;
	tModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tModelDesc.strModelProtoName, "60100001_chocoduckyball.model");
	//tModelDesc.eShaderProtoLevelID = LEVEL_LOADING;
	//strcpy_s(tModelDesc.strShaderProtoName, "Prototype_Component_Shader_VtxAnimMesh");
	m_pBird = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	Add_Child(m_pBird);
	m_pBird->Set_AnimationLoop(0, true);
	m_pBird->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_vBirdOffset);

	return S_OK;
}
HREFTYPE CBuilder::Ready_Preview(const _char* szModelTag)
{
	CTerrainObject::TERRAINOBJ_DESC tTerrObjDesc;
	tTerrObjDesc.fRotationPerSec = 5.f;
	tTerrObjDesc.fSpeedPerSec = 1.f;
	tTerrObjDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tTerrObjDesc.strModelProtoName, szModelTag);
	//tTerrObjDesc.eShaderProtoLevelID = LEVEL_LOADING;
	//strcpy_s(tTerrObjDesc.strShaderProtoName, "Prototype_Component_Shader_VtxMesh");
	XMStoreFloat4(&tTerrObjDesc.pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_vPreviewOffset);

	m_pPreview = static_cast<CBuildPreview*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ,LEVEL_HOME, CBuildPreview::m_szProtoTag, &tTerrObjDesc));
	Add_Child(m_pPreview);
	m_pPreview->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_vPreviewOffset);
	m_pPreview->Get_Transform()->Scaling(0.5f, 0.5f, 0.5f);
	return S_OK;
}

void CBuilder::Receive_KeyInput(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyState(KEY::RIGHT) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_E);
	if (m_pGameInstance->GetKeyState(KEY::UP) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_N);
	if (m_pGameInstance->GetKeyState(KEY::DOWN) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_S);
	if (m_pGameInstance->GetKeyState(KEY::LEFT) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_W);
	if (m_pGameInstance->GetKeyState(KEY::W) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_U);
	if (m_pGameInstance->GetKeyState(KEY::S) == KEY_STATE::PRESSING)
		m_vMoveDir += Get_Direction_Vector(DIR_D);


	if (m_pGameInstance->GetKeyState(KEY::R) == KEY_STATE::DOWN)//ȸ��
	{
		_float4 pos;
		XMStoreFloat4(&pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_uint index = m_pCubeTerrain->PosToIndex(pos);
		CTerrainObject* pTerrObj = m_pCubeTerrain->Get_TerrainObject(index);
		if (pTerrObj)
			pTerrObj->Rotate();
		else
			m_pPreview->Rotate();
	}
	if (m_pGameInstance->GetKeyState(KEY::E) == KEY_STATE::DOWN) // ȸ��
	{
		_float4 pos;
		XMStoreFloat4(&pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_uint index = m_pCubeTerrain->PosToIndex(pos);
		m_pCubeTerrain->Remove_TerrainObject(index);

	}
	if (m_pGameInstance->GetKeyState(KEY::SPACE) == KEY_STATE::DOWN) // ��ġ
	{
		CTerrainObject::TERRAINOBJ_DESC desc;
		desc.fRotationPerSec = 5.f;
		desc.fSpeedPerSec = 1.f;
		desc.eType = TERRAIN_OBJ_TYPE::BLOCK;
		strcpy_s(desc.strItemName, m_szItemName);
		ITEM_DESC* itemdesc = ITEMDB->GetItemDesc(ITEM_TYPE::BUILD, desc.strItemName);
		strcpy_s(desc.strModelProtoName, itemdesc->strModelTag);
		desc.eModelProtoLevelID = LEVEL_LOADING;
		//strcpy_s(desc.strShaderProtoName, ("Prototype_Component_Shader_VtxMesh"));
		//desc.eShaderProtoLevelID = LEVEL_LOADING;
		desc.direction = m_pPreview->Get_Direction();
		desc.data = m_iBuildData;
		_float4 pos;
		XMStoreFloat4(&pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		desc.index = m_pCubeTerrain->PosToIndex(pos);
		desc.pos = m_pCubeTerrain->IndexToPos(desc.index);

		m_pCubeTerrain->Add_TerrainObject(desc);
	}
}

void CBuilder::Update(_float fTimeDelta)
{
	XMVECTOR vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	XMVECTOR vBirdPos = vPos + m_vBirdOffset;
	//m_pBird->Get_Transform()->Set_State(CTransform::STATE_POSITION, vBirdPos);

	XMVECTOR vPreviewPos = vPos + m_vPreviewOffset;
	//m_pPreview->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPreviewPos);
	if (false == XMVector4Equal(m_vMoveDir, XMVectorSet(0, 0, 0, 0)))
	{
		m_pTransformCom->Go_Direction(m_vMoveDir, fTimeDelta);

		//m_pBird->Get_Transform()->LookToward(XMVectorSetY( m_vMoveDir,0));
		m_pTransformCom->LookToward(XMVectorSetY(m_vMoveDir, 0));
	}
	__super::Update(fTimeDelta);
}

void CBuilder::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_vMoveDir = XMVectorSet(0, 0, 0, 0);
}



void CBuilder::Set_BuildItem(const ITEM_DESC* tItemDesc)
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

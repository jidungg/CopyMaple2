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
#include "EffModelObject.h"

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
;
	if (FAILED(Ready_Preview(static_cast<BUILD_ITEM_DATA*>( ITEMDB->Get_Data(ITEM_TYPE::BUILD, 0)))))
		return E_FAIL;

	if (FAILED(Ready_Marker()))
		return E_FAIL;
	return S_OK;
}
HRESULT CBuilder::Ready_Builder()
{
	//DuckyBall
	CModelObject::MODELOBJ_DESC tModelDesc;
	tModelDesc.fRotationPerSec = 5.f;
	tModelDesc.fSpeedPerSec = 1.f;
	tModelDesc.eModelProtoLevelID = LEVEL_HOME;
	strcpy_s(tModelDesc.strModelProtoName, "60100001_chocoduckyball.model");
	m_pBird = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	Add_Child(m_pBird);
	m_pBird->Set_AnimationLoop(0, true);
	m_pBird->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_vBirdOffset);

	return S_OK;
}
HRESULT CBuilder::Ready_Preview(BUILD_ITEM_DATA* pDesc)
{
	CTerrainObject::TERRAINOBJ_DESC tTerrObjDesc;
	tTerrObjDesc.fRotationPerSec = 5.f;
	tTerrObjDesc.fSpeedPerSec = 1.f;
	tTerrObjDesc.iID =pDesc->iItemID;
	tTerrObjDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tTerrObjDesc.strModelProtoName, pDesc->strModelTag);
	XMStoreFloat4(&tTerrObjDesc.pos, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_vPreviewOffset);

	m_pPreview = static_cast<CBuildPreview*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ,LEVEL_HOME, CBuildPreview::m_szProtoTag, &tTerrObjDesc));
	Add_Child(m_pPreview);
	m_pPreview->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_vPreviewOffset);
	m_pPreview->Get_Transform()->Scaling(0.5f, 0.5f, 0.5f);
	return S_OK;
}

HRESULT CBuilder::Ready_Marker()
{
	CEffModelObject::EFFECTOBJ_DESC tEffModelDesc;
	strcpy_s( tEffModelDesc.strModelProtoName ,"eff_add_cube_mark_enable.effmodel");
	tEffModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	m_pCubeMarkerEnable = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffModelDesc));

	strcpy_s(tEffModelDesc.strModelProtoName, "eff_add_cube_mark_disable.effmodel");
	tEffModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	m_pCubeMarkerDisable = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffModelDesc));


	m_pCubeMarkerEnable->Start_Animation(0,true);
	m_pCubeMarkerDisable->Start_Animation(0, true);

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

#ifdef _DEBUG
	for (_uint key = 0; key < 10; key++)
	{
		if (m_pGameInstance->GetKeyState((KEY)((_uint)KEY::NUM0 + key)) == KEY_STATE::DOWN)
		{
			m_iBuildData = key;
			break;
		}
	}
	if (m_pGameInstance->GetKeyState(KEY::F1) == KEY_STATE::PRESSING)
		m_fBuildData -= m_fBuildDataDelta;
	if (m_pGameInstance->GetKeyState(KEY::F2) == KEY_STATE::PRESSING)
		m_fBuildData += m_fBuildDataDelta;
#endif

	_vector vPos;
	vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_uint iIndex = m_pCubeTerrain->PosToIndex(vPos);
	m_bBuildable = m_pCubeTerrain->Is_Buildable(vPos);
	if (m_pGameInstance->GetKeyState(KEY::R) == KEY_STATE::DOWN)//ȸ��
	{
		CTerrainObject* pTerrObj = m_pCubeTerrain->Get_TerrainObject(iIndex);
		if (pTerrObj)
			pTerrObj->Rotate();
		else
			m_pPreview->Rotate();
	}
	if (m_pGameInstance->GetKeyState(KEY::E) == KEY_STATE::DOWN) // ȸ��
	{

		m_pCubeTerrain->Remove_TerrainObject(iIndex);

	}
	if (m_bBuildable && m_pGameInstance->GetKeyState(KEY::SPACE) == KEY_STATE::PRESSING) // ��ġ
	{
		BUILD_ITEM_DATA* itemdesc =static_cast<BUILD_ITEM_DATA*>( ITEMDB->Get_Data(ITEM_TYPE::BUILD, (_uint)m_pPreview->Get_BuildItemID()));

		CTerrainObject::TERRAINOBJ_DESC desc;
		desc.fRotationPerSec = 5.f;
		desc.fSpeedPerSec = 1.f;
		desc.iID = m_pPreview->Get_BuildItemID();
		desc.eModelProtoLevelID = LEVEL_LOADING;
		strcpy_s(desc.strModelProtoName, itemdesc->strModelTag);
		desc.direction = m_pPreview->Get_Direction();
		desc.vecIData.push_back(m_iBuildData);
		desc.vecFData.push_back(m_fBuildData);
		desc.index = iIndex;
		XMStoreFloat4(&desc.pos, m_pCubeTerrain->IndexToPos(desc.index));

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

	_vector vSnapPos = m_pCubeTerrain->SnapPosition(vPos);
	if (m_bBuildable)
	{
		m_pCubeMarkerDisable->Set_Active(false);
		m_pCubeMarkerEnable->Set_Active(true);
		m_pCubeMarkerEnable->Get_Transform()->Set_State(CTransform::STATE_POSITION, vSnapPos);
		m_pCubeMarkerEnable->Update(fTimeDelta);
	}
	else
	{
		m_pCubeMarkerEnable->Set_Active(false);
		m_pCubeMarkerEnable->Set_Active(true);
		m_pCubeMarkerDisable->Get_Transform()->Set_State(CTransform::STATE_POSITION, vSnapPos);
		m_pCubeMarkerDisable->Update(fTimeDelta);
	}

	__super::Update(fTimeDelta);
}



void CBuilder::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_vMoveDir = XMVectorSet(0, 0, 0, 0);

	if (m_bBuildable)
		m_pCubeMarkerEnable->Late_Update(fTimeDelta);
	else
		m_pCubeMarkerDisable->Late_Update(fTimeDelta);

}
HRESULT CBuilder::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	if (m_bBuildable)
		m_pCubeMarkerEnable->Render();
	else
		m_pCubeMarkerDisable->Render();
	return S_OK;
}


void CBuilder::Set_BuildItem(_uint eID)
{
	BUILD_ITEM_DATA* tItemDesc = static_cast<BUILD_ITEM_DATA*>(ITEMDB->Get_Data(ITEM_TYPE::BUILD, (_uint)eID));

	Remove_Child(m_pPreview);
	Safe_Release(m_pPreview);
	Ready_Preview(tItemDesc);
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

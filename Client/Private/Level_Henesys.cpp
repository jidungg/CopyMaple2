#include "stdafx.h"
#include "Level_Henesys.h"
#include "GameInstance.h"
#include "AttachableBodyPart.h"
#include "Client_Utility.h"
#include "Collider_Sphere.h"

CLevel_Henesys::CLevel_Henesys(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel_GamePlay(pDevice, pContext)
{
}

void CLevel_Henesys::On_Start(_uint iPrevLevelID)
{
	__super::On_Start(iPrevLevelID);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_PLAYER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_MONSTER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_TERRAIN, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_PLAYER, LAYERID::LAYER_INTERACTION, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_MONSTER, true);
	m_pGameInstance->Set_CollisionMatrix(LAYERID::LAYER_BULLET, LAYERID::LAYER_PLAYER, true);

	XMStoreFloat4x4(&m_matTmp, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matTmpSocket, XMMatrixIdentity());
	m_matTmp._41 = 1.f;
	m_matTmp._42 = 1.f;
	m_matTmp._43 = 1.f;
	m_matTmpSocket._42 = 0.5f;

#ifdef _DEBUG
	CAttachableBodyPart::ATTACHABLE_BODYPART_DESC tDesc = {};
	tDesc.pParentMatrix = &m_matTmp;
	CCollider_Sphere::SPHERE_COLLIDER_DESC tColliderDesc = {};

	//BODY
	tColliderDesc.fRadius = 0.5;
	tColliderDesc.vCenter = { 0,0,0  };
	tDesc.tColliderDesc = tColliderDesc;
	tDesc.pSocketMatrix = &m_matTmpSocket;
	 
	m_pGameInstance->Add_GameObject_ToLayer(Get_CurrentTrueLevel(), LAYER_INTERACTION, static_cast<CAttachableBodyPart*>(m_pGameInstance->Clone_Proto_Object_Stock(CAttachableBodyPart::m_szProtoTag, &tDesc)));
#endif
}

CLevel_Henesys* CLevel_Henesys::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CLevel_Henesys* pInstance = new CLevel_Henesys(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CLevel_Henesys");
		Safe_Release(pInstance);
	}

	return pInstance;
}

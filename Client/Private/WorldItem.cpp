#include "stdafx.h"
#include "WorldItem.h"
#include "GameInstance.h"
#include "Client_Utility.h"
#include "CubeTerrain.h"
#include "Engine_Utility.h"
#include "EffModel.h"


CWorldItem::CWorldItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffModelObject(pDevice, pContext)
{
}

CWorldItem::CWorldItem(const CEffModelObject& Prototype)
	: CEffModelObject(Prototype)
{
}

HRESULT CWorldItem::Initialize(void* pArg)
{
	WORLDITEM_DESC* pDesc = (WORLDITEM_DESC*)pArg;
	m_pTerrain = pDesc->pTerrain;
	m_pItemData = pDesc->pItemData;
	m_iStackCount = pDesc->iStackCount;


	ITEM_GRADE eItemGrade = m_pItemData->eItemGrade;

	switch (eItemGrade)
	{
	case Client::ITEM_GRADE::NORMAL:
		strcpy_s(pDesc->strModelProtoName, "eff_drop_rare.effmodel");
		break;
	case Client::ITEM_GRADE::ELITE:
		strcpy_s(pDesc->strModelProtoName, "eff_drop_excellent.effmodel");
		break;
	case Client::ITEM_GRADE::EXCELENT:
		strcpy_s(pDesc->strModelProtoName, "eff_drop_elite.effmodel");
		break;
	case Client::ITEM_GRADE::LEGENDARY:
		strcpy_s(pDesc->strModelProtoName, "eff_drop_legendary.effmodel");
		break;
	case Client::ITEM_GRADE::MYTHIC:
		strcpy_s(pDesc->strModelProtoName, "eff_drop_artifact.effmodel");
		break;
	case Client::ITEM_GRADE::LAST:
		break;
	default:
		break;
	}
	pDesc->eModelProtoLevelID = LEVEL_LOADING;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	string strProtoItemIconTag = m_pItemData->strIconImageTag;
	wstring wstrItemIconTag(strProtoItemIconTag.begin(), strProtoItemIconTag.end());
	CTexture* pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Proto_Component_Stock(wstrItemIconTag));
	m_pModelCom->Set_Texture(0, pTexture);
	Compute_Matrix();
	Start_Animation(0, true);
	return S_OK;
}

void CWorldItem::Update(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyState(KEY::F2) == KEY_STATE::DOWN)
		PopUp();
	_vector vCurrentPosition = Get_WorldPosition();
	if (m_bPopUp)
	{
		vCurrentPosition.m128_f32[1] += m_fUpForce * fTimeDelta;
		vCurrentPosition += m_vMoveDirection * m_fMoveSpeed* fTimeDelta;

		m_fUpForce -= 9.8f * 3 * fTimeDelta;
		_float fFloorHeight = m_pTerrain->Get_FloorHeight(vCurrentPosition);
		if (vCurrentPosition.m128_f32[1] <= fFloorHeight)
		{
			vCurrentPosition.m128_f32[1] = fFloorHeight;
			m_bPopUp = false;
			m_fUpForce = 0;
		}
	}
	else
	{
		_float fFloorHeight = m_pTerrain->Get_FloorHeight(vCurrentPosition);
		vCurrentPosition.m128_f32[1] += m_fUpForce * fTimeDelta;
		m_fUpForce += m_fUpforceAcc * fTimeDelta;
		if (fabs( m_fUpForce) > m_fUpforceMax)
		{
			m_fUpforceAcc *= -1;
		}
		m_pTransformCom->Turn({0,1,0,0}, fTimeDelta);
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurrentPosition);
	__super::Update(fTimeDelta);
}

void CWorldItem::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

void CWorldItem::PopUp()
{
	m_bPopUp = true;
	m_pTerrain = static_cast<CCubeTerrain*>(m_pGameInstance->Get_FirstGameObject(Get_CurrentTrueLevel(), LAYERID::LAYER_TERRAIN));
	m_fUpForce = m_fJumpForce;
	m_vMoveDirection.m128_f32[0] = CEngineUtility::Get_RandomFloat(-1, 1);
	m_vMoveDirection.m128_f32[2] = CEngineUtility::Get_RandomFloat(-1, 1);
	m_vMoveDirection = XMVector3Normalize(m_vMoveDirection);
}

void CWorldItem::Set_Item(const ITEM_DATA* pItemData, _uint iStackCount )
{


}

CWorldItem* CWorldItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWorldItem* pInstance = new CWorldItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWorldItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWorldItem::Clone(void* pArg)
{
	CWorldItem* pInstance = new CWorldItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWorldItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWorldItem::Free()
{
	__super::Free();
}

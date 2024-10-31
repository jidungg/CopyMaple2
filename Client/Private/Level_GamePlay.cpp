#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Player.h"
#include "Camera_Trace.h"
#include "Camera_Free.h"
#include "Level_Loading.h"
#include "UIQuickSlot.h"
#include "SkillManager.h"
#include "Bayar.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	m_iLevelID = LEVEL_GAMEPLAY;
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;
	m_pPlayer = static_cast<CPlayer*>( m_pGameInstance->Get_FirstGameObject(LEVEL_LOADING, TEXT("Layer_Player")));
	m_pGameInstance->Possess(m_pPlayer);
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;
	if (FAILED(Ready_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;
	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Lights()
{


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Henesys"), LEVEL_GAMEPLAY, strLayerTag, (void*)nullptr)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Camera(const _wstring& strLayerTag)
{


	CCamera_Trace::TRACECAMERA_DESC		CamDesc{};

	CamDesc.fSpeedPerSec = 5.f;
	CamDesc.fRotationPerSec = XMConvertToRadians(90.f);
	CamDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CamDesc.fFovY = XMConvertToRadians(60.f);
	CamDesc.fNear = 0.1f;
	CamDesc.fFar = 1000.f;
	CamDesc.vEye = _float3(0.f, 3.f, 3.f);
	CamDesc.vAt = _float3(0.f, 0.f, 0.f);
	CamDesc.vArm = _float3(-2.f, 2.f, -2.f);
	CamDesc.pTarget = m_pPlayer;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_LOADING, TEXT("Prototype_GameObject_Camera_Trace"),
		LEVEL_GAMEPLAY, strLayerTag, &CamDesc)))
		return E_FAIL;

#pragma region Test

	//Body
	//CModelObject::MODELOBJ_DESC tModelDesc;
	//tModelDesc.fRotationPerSec = 5.f;
	//tModelDesc.fSpeedPerSec = 1.f;
	//tModelDesc.eModelType = CModel::TYPE_NONANIM;
	//tModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	//strcpy_s(tModelDesc.strModelProtoName, "Prototype_Model_Player");
	//tModelDesc.eShaderProtoLevelID = LEVEL_LOADING;
	//strcpy_s(tModelDesc.strShaderProtoName, "Prototype_Component_Shader_VtxMesh");
	//m_pModel = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, strLayerTag, m_pModel)))
	//	return E_FAIL;
	//m_pModel->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 1.f, 0.f, 1.f));

	//

	//tModelDesc.fRotationPerSec = 5.f;
	//tModelDesc.fSpeedPerSec = 1.f;
	//tModelDesc.eModelType = CModel::TYPE_NONANIM;
	//tModelDesc.eModelProtoLevelID = LEVEL_LOADING;
	//strcpy_s(tModelDesc.strModelProtoName, "00200007_f_jujuwave_p_a.model");
	//tModelDesc.eShaderProtoLevelID = LEVEL_LOADING;
	//strcpy_s(tModelDesc.strShaderProtoName, "Prototype_Component_Shader_VtxMesh");
	//m_pHair = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	//if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, strLayerTag, m_pHair)))
	//	return E_FAIL;
	//m_pHair->Get_Transform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 1.f, 0.f,1.f));

#pragma endregion
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UI(const _wstring& strLayerTag)
{
	CUIQuickSlot::QUICKLSLOT_DESC quickSlotDesc;
	quickSlotDesc.eAnchorType = CORNOR_TYPE::LEFT_TOP;
	quickSlotDesc.ePivotType = CORNOR_TYPE::LEFT_TOP;
	quickSlotDesc.fSizeX = 60;
	quickSlotDesc.fSizeY = 60;
	quickSlotDesc.fXOffset = 100;
	quickSlotDesc.fYOffset = 100;
	quickSlotDesc.eHotKey = KEY::Q;

	m_pQuickSlot = static_cast<CUIQuickSlot*>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, LEVEL_LOADING, CUIQuickSlot::m_szProtoTag, &quickSlotDesc));
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, strLayerTag, m_pQuickSlot)))
		return E_FAIL;
	m_pQuickSlot->Set_QuickItem(m_pPlayer->Get_Skill(SKILL_ID::MAGIC_CLAW));
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Monster(const _wstring& strLayerTag)
{
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Bayar"),LEVEL_GAMEPLAY, strLayerTag);

	return S_OK;
}
void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyState(KEY::T) == KEY_STATE::UP)
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_HOME));
	//m_pGameInstance->Add_RenderObject(CRenderer::RENDERGROUP::RG_NONBLEND, m_pHair);
	//m_pGameInstance->Add_RenderObject(CRenderer::RENDERGROUP::RG_NONBLEND, m_pModel);
}

HRESULT CLevel_GamePlay::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("헤네시스"));
#endif

	return S_OK;
}


CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}

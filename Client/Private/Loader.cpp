#include "stdafx.h"
#include "..\Public\Loader.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "JsonParser.h"
#include "DropTable.h"
#include "NPCDataBase.h"
#include "NPC.h"
#include "QuestDataBase.h"

#include "Camera_Free.h"
#include "Camera_Trace.h"
#include "CubeTerrain.h"
#include "TerrainObject.h"
#include "Player.h"
#include "RenderObject.h"
#include "MeshCollider.h"
#include "Builder.h"
#include "ModelObject.h"
#include "ItemDataBase.h"

#include "UIHomeDialog.h"
#include "UIPanel.h"
#include "UIItemIndicator.h"
#include "UIList.h"
#include "UIQuickSlot.h"
#include "UIBundle.h"
#include "UIQuickSlotBundle.h"
#include "UIBar.h"
#include "UIInventory.h"
#include "UIScroller.h"
#include "UIScrollButton.h"
#include "UIDamgCount.h"
#include "UIInvenSlotEntry.h"
#include "UIFont.h"
#include "UIIcon.h"
#include "BackGround.h"
#include "UIInvenTabButton.h"
#include "UIInvenItemList.h"
#include "UIMainHUDGuage.h"
#include "UIVerticalFill.h"
#include "UIPlayerInfo.h"
#include "UIPlayerInfoSlot.h"
#include "UIModelPad.h"
#include "UINPCDialog.h"
#include "UIChatOption.h"

#include "StateMachine.h"
#include "SkillManager.h"
#include "Character.h"
#include "BuildPreview.h"
#include "BoneModelObject.h"
#include "Weapon.h"
#include "MimicBoneModel.h"
#include "Face.h"
#include "HumanModelObject.h"
#include "Bayar.h"


#include "Collider_Sphere.h"
#include "Collider_AABB.h"
#include "Collider_Frustum.h"
#include "Collider_Cylinder.h"
#include "MonsterDataBase.h"

#include "Bullet_MagicClaw.h"
#include "Bullet_Kindling.h"
#include "Bullet_FireTornado.h"
#include "Bullet_BBQParty.h"
#include "Bullet_FakeMeteor.h"
#include "Bullet_WildFire.h"
#include "Bullet_BayarAttackB.h"
#include "Bullet_BayarAttackD.h"
#include "Bullet_BayarStoneSpike.h"

#include "MonsterSpawner.h"
#include "PortalTerrainObject.h"
#include "Portal.h"
#include "WayFinder.h"
#include "EffModel.h"
#include "EffModelObject.h"
#include "EffectManager.h"
#include "AttachableBodyPart.h"
#include "WorldItem.h"
#include "NPCSpanwer.h"


CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);

}

//typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
_uint APIENTRY LoadingMain(void* pArg)
{	
	CLoader*	pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	CoInitializeEx( nullptr, 0);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_Level_Logo();
		break;
	case LEVEL_HENESYS:
		hr = Loading_Level_Henesys();
		break;
	case LEVEL_BAYARPEAK:
		hr = Loading_Level_BayarPeak();
		break;
	case LEVEL_HOME:
		hr = Loading_Level_MyHome();
		break;
	case LEVEL_HUNTINGPLACE:
		hr = Loading_Level_HuntingPlace();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	return S_OK;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

#ifdef _DEBUG

void CLoader::Show_Debug()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

#endif


HRESULT CLoader::Loading_Level_Logo()
{

	if (FAILED(SKILLDB->Load_Data()))
		return E_FAIL;
	if (FAILED(MONSTERDB->Load_Data()))
		return E_FAIL;
	if (FAILED(NPCDB->Load_Data()))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("폰트 로드."));
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_15"), TEXT("../Bin/Resources/Fonts/Lev2_15.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_30"), TEXT("../Bin/Resources/Fonts/Lev2_30.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_Bold_10"), TEXT("../Bin/Resources/Fonts/Lev2_Bold_10.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_Bold_13"), TEXT("../Bin/Resources/Fonts/Lev2_Bold_13.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_Bold_15"), TEXT("../Bin/Resources/Fonts/Lev2_Bold_15.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_Bold_30"), TEXT("../Bin/Resources/Fonts/Lev2_Bold_30.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_Medium_10"), TEXT("../Bin/Resources/Fonts/Lev2_Medium_10.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_Medium_13"), TEXT("../Bin/Resources/Fonts/Lev2_Medium_13.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_Medium_15"), TEXT("../Bin/Resources/Fonts/Lev2_Medium_15.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("LV2Gothic_Medium_30"), TEXT("../Bin/Resources/Fonts/Lev2_Medium_30.spritefont"))))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("텍스처 로드."));
	/* For.Prototype_Component_Texture_Logo */
	//히ㅏ다보니까 다 STatic 됨
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Texture_QuickSlot_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/QuickSlot_Normal.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Texture_InvenSlotBack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/SlotBack/InvenSlot_%d.dds"), 5))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Texture_PlayerInfoSlotBack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/PlayerInfo/playerinfo_slot_base_%d.dds"), 5))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Face_Face1"),
		CFace::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Face/00300011/")))))
		return E_FAIL;

	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/Icon/Skill/"), TEXT(".png"))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/Icon/Item/"), TEXT(".png"))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/Icon/"), TEXT(".png"))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/UI/Inventory/"), TEXT(".dds"))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/UI/CastingBar/"), TEXT(".dds"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_HighlightBorder"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Home/Home_Dialog_Mat_HighlightBorder.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_CloseButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/close_button_%d.dds"), 4))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_ScrollUpButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/ScrollBar/scroll_up_button_%d.dds"), 4))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_ScrollDownButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/ScrollBar/scroll_down_button_%d.dds"), 4))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_ScrollButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/ScrollBar/scroll_button_%d.dds"), 4))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_DamagCountPlayerNormal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/dmgcnt_player_normal.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_DamagCountPlayerCritical"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/dmgcnt_player_critical.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_DamagCountMonsterNormal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/dmgcnt_monster_normal.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_DamagCountMonsterCritical"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/dmgcnt_monster_critical.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_DamagCountHealNormal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/dmgcnt_heal_normal.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_ItemListBack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Home/Home_Dialog_ItemListBack.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_TabButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/Inventory_TabButton_%d.dds"), 3))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_NPCDialogButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPCDialog/Button/npcdialog_button_%d.dds"), 4))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("UI_Texture_NPCDialogOptionButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NPCDialog/OptionButton/npcdialog_opt_button_%d.dds"), 4))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/"), TEXT(".dds"))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/UI/MainHPBar/"), TEXT(".dds"))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/UI/PlayerInfo/"), TEXT(".dds"))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/Resources/Textures/UI/NPCDialog/ETC/"), TEXT(".dds"))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("셰이더 로드."));
#pragma region Shader
	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxMesh*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_VtxHumanAnimMesh*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxHumanAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxHumanAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_UI*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_UI.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	/* For.Prototype_Component_Shader_EffectLMesh*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxEffectMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/shaderFiles/Shader_VtxEffectMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("모델 로드."));
#pragma region Model
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CVIBuffer_Rect::m_szPrptotypeTag,
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_Component_CModel*/
	XMMATRIX matPretransform = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	if (FAILED(Load_Dirctory_Models_Recursive(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/NonAnim"), CModel::TYPE_NONANIM, matPretransform)))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Models_Recursive(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/Anim/Equip"), CModel::TYPE_ANIM, matPretransform)))
		return E_FAIL;


	  matPretransform = matPretransform * XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(Load_Dirctory_Models_Recursive(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/Anim/Player"),CModel::TYPE_ANIM,matPretransform)))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Models_Recursive(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/Anim/Monster"), CModel::TYPE_ANIM, matPretransform)))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Models_Recursive(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/Mimic"), CModel::TYPE_MIMIC, matPretransform)))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Models(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/MAP/Field/"), matPretransform)))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Models(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/MAP/Cube/"), matPretransform)))
		return E_FAIL;

	XMMATRIX matPretransform2 = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	matPretransform2 = matPretransform2 * XMMatrixRotationX(XMConvertToRadians(-90));
	matPretransform2 = matPretransform2 * XMMatrixRotationY(XMConvertToRadians(180));
	matPretransform2 = matPretransform2 * XMMatrixScaling(-1,1,1);
	if (FAILED(Load_Dirctory_EffModels(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/Effect"), matPretransform2)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CCollider_Sphere::m_szProtoTag,
		CCollider_Sphere::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CCollider_AABB::m_szProtoTag,
		CCollider_AABB::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CCollider_Mesh::m_szProtoTag,
		CCollider_Mesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CCollider_Frustum::m_szProtoTag,
		CCollider_Frustum::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CCollider_Cylinder::m_szProtoTag,
		CCollider_Cylinder::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//지금 LEVEL_LOADING을 static으로 쓰고있음 그냥 안없어짐 ㅋㅋ


#pragma endregion
	lstrcpy(m_szLoadingText, TEXT("객체 로드."));
#pragma region Object
	//ModelObject
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CModelObject::m_szProtoTag,
		CModelObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CEffModelObject::m_szProtoTag,
		CEffModelObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBoneModelObject::m_szProtoTag,
		CBoneModelObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CWeapon::m_szProtoTag,
		CWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING,
		CTerrainObject::m_szProtoTag,
		CTerrainObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CPortal::m_szProtoTag,
		CPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CPortalTerrainObject::m_szProtoTag,
		CPortalTerrainObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//BULLETS
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_MagicClaw::m_szProtoTag,
		CBullet_MagicClaw::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_Kindling::m_szProtoTag,
		CBullet_Kindling::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_FireTornado::m_szProtoTag,
		CBullet_FireTornado::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_BBQParty::m_szProtoTag,
		CBullet_BBQParty::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_FakeMeteor::m_szProtoTag,
		CBullet_FakeMeteor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_WildFire::m_szProtoTag,
		CBullet_WildFire::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_BayarAttackB::m_szProtoTag,
		CBullet_BayarAttackB::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_BayarAttackD::m_szProtoTag,
		CBullet_BayarAttackD::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBullet_BayarStoneSpike::m_szProtoTag,
		CBullet_BayarStoneSpike::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_BackGround */

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIPanel::m_szProtoTag,
		CUIPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CBackGround::m_szProtoTag,
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_UIList"),
		CUIListSelector::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_CUIInvenItemList"),
		CUIInvenItemList::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIQuickSlot::m_szProtoTag,
		CUIQuickSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIQuickSlotBundle::m_szProtoTag,
		CUIQuickSlotBundle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIInventory::m_szProtoTag,
		CUIInventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIButton::m_szProtoTag,
		CUIButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIBar::m_szProtoTag,
		CUIBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIScroller::m_szProtoTag,
		CUIScroller::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIScrollButton::m_szProtoTag,
		CUIScrollButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIDamgCount::m_szProtoTag,
		CUIDamgCount::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIFont::m_szProtoTag,
		CUIFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIInvenSlotEntry::m_szProtoTag,
		CUIInvenSlotEntry::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIIcon::m_szProtoTag,
		CUIIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIInvenTabButton::m_szProtoTag,
		CUIInvenTabButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIMainHUDGuage::m_szProtoTag,
		CUIMainHUDGuage::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_UI_IntVerticalFill"),
		CUIVerticalFill<_int>::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIPlayerInfo::m_szProtoTag,
		CUIPlayerInfo::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIPlayerInfoSlot::m_szProtoTag,
		CUIPlayerInfoSlot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIModelPad::m_szProtoTag,
		CUIModelPad::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUINPCDialog::m_szProtoTag,
		CUINPCDialog::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIChatOption::m_szProtoTag,
		CUIChatOption::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CHumanModelObject::m_szProtoTag,
		CHumanModelObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Camera_Trace */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_Camera_Trace"),
		CCamera_Trace::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CMonster::m_szProtoTag,
		CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CMonsterSpawner::m_szProtoTag,
		CMonsterSpawner::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CNPC::m_szProtoTag,
		CNPC::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CNPCSpanwer::m_szProtoTag,
		CNPCSpanwer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CAttachableBodyPart::m_szProtoTag,
		CAttachableBodyPart::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CWorldItem::m_szProtoTag,
		CWorldItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


	if (FAILED(ITEMDB->Load_Data()))
		return E_FAIL;
	if (FAILED(EFFECT_MANAGER->Load_Data()))
		return E_FAIL;
	if (FAILED(DROPTABLE->Load_Data()))
		return E_FAIL;
	if (FAILED(QUESTDB->Load_Data()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로드 완료."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_Henesys()
{
	lstrcpy(m_szLoadingText, TEXT("텍스처."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HENESYS, TEXT("bg_henesys_a.dds"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BackGround/bg_henesys_a.dds"), 1))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델 로드"));
	XMMATRIX matPretransform = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	matPretransform = matPretransform * XMMatrixRotationY(XMConvertToRadians(180.f)); 
	if (FAILED(Load_Dirctory_Models_Recursive(LEVEL_HENESYS,
		TEXT("../Bin/resources/FBXs/Anim/EnchantMaster"), CModel::TYPE_ANIM, matPretransform)))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("객체 로드"));

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HENESYS, TEXT("Prototype_GameObject_Henesys"),
		CCubeTerrain::Create(m_pDevice, m_pContext, ("../Bin/Resources/Json/Henesys.json")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩 완료"));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_BayarPeak()
{

	lstrcpy(m_szLoadingText, TEXT("텍스처 로드"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BAYARPEAK, TEXT("bg_perion_c.dds"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BackGround/bg_perion_c.dds"), 1))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("모델 로드."));
	XMMATRIX matPretransform = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	matPretransform = matPretransform * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(Load_Dirctory_Models_Recursive(LEVEL_BAYARPEAK,
		TEXT("../Bin/resources/FBXs/Anim/Boss"), CModel::TYPE_ANIM, matPretransform)))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("객체 로드."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BAYARPEAK, TEXT("Prototype_GameObject_BayarPeak"),
		CCubeTerrain::Create(m_pDevice, m_pContext, ("../Bin/Resources/Json/BayarPeak.json")))))
		return E_FAIL;
	/* BAYAR */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_BAYARPEAK, TEXT("Prototype_GameObject_Bayar"),
		CBayar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로드 완료."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_MyHome()
{
	lstrcpy(m_szLoadingText, TEXT("텍스처 로드"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("UI_Texture_Magnifier"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Home/Home_Dialog_Magnifier_%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("UI_Texture_HomeDialog"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Home/Home_Dialog_Box_%d.dds"), 4))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("UI_Texture_SelectedYellow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Home/Home_Dialog_Mat_SelectedYellow.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("bg_indoor_a.dds"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BackGround/bg_indoor_a.dds"), 1))))
		return E_FAIL;




	lstrcpy(m_szLoadingText, TEXT("모델 로드."));
	XMMATRIX matPretransform = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	matPretransform = matPretransform * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(Load_Dirctory_Models_Recursive(LEVEL_HOME,
		TEXT("../Bin/resources/FBXs/Anim/DuckyBall"), CModel::TYPE_ANIM, matPretransform)))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("객체 로드."));
	/* For.Prototype_GameObject_HomeDialog */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, CUIHomeDialog::m_szProtoTag,
		CUIHomeDialog::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("Prototype_GameObject_MyHome"),
		CCubeTerrain::Create(m_pDevice, m_pContext, ("../Bin/Resources/Json/MyHome.json")))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, CBuilder::m_szProtoTag,
		CBuilder::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, CBuildPreview::m_szProtoTag,
		CBuildPreview::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("Prototype_GameObject_HomeDialogBuildItemIndicator"),
		CUIButtonItemIndicator::Create(m_pDevice, m_pContext,LEVEL_HOME, TEXT("UI_Texture_HomeDialog")))))
		return E_FAIL;





	lstrcpy(m_szLoadingText, TEXT("로드 완료."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_HuntingPlace()
{
	lstrcpy(m_szLoadingText, TEXT("텍스처 로드"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HUNTINGPLACE, TEXT("bg_henesys_b.dds"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BackGround/bg_henesys_b.dds"), 1))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델 로드."));

	lstrcpy(m_szLoadingText, TEXT("객체 로드."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HUNTINGPLACE, TEXT("Prototype_GameObject_HuntingPlace"),
		CCubeTerrain::Create(m_pDevice, m_pContext, ("../Bin/Resources/Json/HuntingPlace.json")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로드 완료."));

	m_isFinished = true;

	return S_OK;
}


HRESULT CLoader::Load_Dirctory_Models_Recursive(LEVELID eLevId,  const _tchar* szDirPath,CModel::TYPE eType ,_fmatrix PreTransformMatrix)
{
	fs::path path;
	path = szDirPath;
	for (const auto& entry : recursive_directory_iterator(path)) {
		if (entry.path().extension() == ".model") {
			//cout << entry.path().string() << endl;

			if (FAILED(m_pGameInstance->Add_Prototype(eLevId, entry.path().filename(),
				CModel::Create(m_pDevice, m_pContext, eType, entry.path().string().c_str(), PreTransformMatrix))))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CLoader::Load_Dirctory_EffModels(LEVELID eLevId, const _tchar* szDirPath, _fmatrix PreTransformMatrix)
{
	fs::path path;
	path = szDirPath;
	for (const auto& entry : recursive_directory_iterator(path)) {
		if (entry.path().extension() == ".effmodel") {
			//cout << entry.path().string() << endl;

			if (FAILED(m_pGameInstance->Add_Prototype(eLevId, entry.path().filename(),
				CEffModel::Create(m_pDevice, m_pContext, entry.path().string().c_str(), PreTransformMatrix))))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CLoader::Load_Dirctory_Models(LEVELID eLevId, const _tchar* szDirPath, _fmatrix PreTransformMatrix)
{
	WIN32_FIND_DATA		FindFileData = {};
	HANDLE				hFind = INVALID_HANDLE_VALUE;

	_tchar				szFilePath[MAX_PATH] = TEXT("");
	_tchar				szFullPath[MAX_PATH] = TEXT("");
	_tchar				szProtoTag[MAX_PATH] = TEXT("");
	_tchar				szExtension[MAX_PATH] = TEXT(".model");

	lstrcpy(szFilePath, szDirPath);
	lstrcat(szFilePath, TEXT("*"));
	lstrcat(szFilePath, szExtension);

	hFind = FindFirstFile(szFilePath, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFind)
		return E_FAIL;

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		lstrcpy(szFullPath, szDirPath);
		lstrcat(szFullPath, FindFileData.cFileName);

		wstring wstr = szFullPath;
		string str{ wstr.begin(), wstr.end() };

		if (FAILED(m_pGameInstance->Add_Prototype(eLevId, FindFileData.cFileName,
			CModel::Create(m_pDevice, m_pContext, str.c_str(), PreTransformMatrix))))
			return E_FAIL;

	} while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);

	return S_OK;
}

HRESULT CLoader::Load_Dirctory_Textures(LEVELID eLevId, const _tchar* szDirPath, const _tchar* szExtension)
{
	WIN32_FIND_DATA		FindFileData = {};
	HANDLE				hFind = INVALID_HANDLE_VALUE;

	_tchar				szFilePath[MAX_PATH] = TEXT("");
	_tchar				szFullPath[MAX_PATH] = TEXT("");
	_tchar				szProtoTag[MAX_PATH] = TEXT("");

	lstrcpy(szFilePath, szDirPath);
	lstrcat(szFilePath, TEXT("*"));
	lstrcat(szFilePath, szExtension);

	hFind = FindFirstFile(szFilePath, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFind)
		return E_FAIL;

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		lstrcpy(szFullPath, szDirPath);
		lstrcat(szFullPath, FindFileData.cFileName);

		wstring wstr = szFullPath;

		if (FAILED(m_pGameInstance->Add_Prototype(eLevId, FindFileData.cFileName,
			CTexture::Create(m_pDevice, m_pContext, wstr.c_str(), 1))))
			return E_FAIL;

	} while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);

	return S_OK;
}


CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);


}

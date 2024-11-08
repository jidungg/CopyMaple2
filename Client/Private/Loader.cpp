#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "JsonParser.h"

#include "Camera_Free.h"
#include "Camera_Trace.h"
#include "UIPanel.h"
#include "CubeTerrain.h"
#include "TerrainObject.h"
#include "Player.h"
#include "RenderObject.h"
#include "UIHomeDialog.h"
#include "Terrain.h"
#include "MeshCollider.h"
#include "Builder.h"
#include "ModelObject.h"
#include "ItemDataBase.h"
#include "UIItemIndicator.h"
#include "UIList.h"
#include "StateMachine.h"

#include "SkillManager.h"
#include "UIQuickSlot.h"
#include "Character.h"
#include "BuildPreview.h"
#include "BoneModelObject.h"
#include "Weapon.h"
#include "MimicBoneModel.h"
#include "Face.h"
#include "HumanModelObject.h"
#include "Bayar.h"

#include "Engine_Defines.h"
#include "UIInventory.h"
#include "Collider_Sphere.h"
#include "UIBundle.h"
#include "UIQuickSlotBundle.h"
#include "Collider_AABB.h"
#include "MonsterDataBase.h"

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
	case LEVEL_GAMEPLAY:
		hr = Loading_Level_GamePlay();
		break;
	case LEVEL_HOME:
		hr = Loading_Level_MyHome();
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
	if (FAILED(ITEMDB->Load_Data()))
		return E_FAIL;
	if (FAILED(Load_SkillData()))
		return E_FAIL;
	if (FAILED(MONSTERDB->Load_Data()))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("텍스처 로드."));
	/* For.Prototype_Component_Texture_Logo */
   
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BackGround/bg_henesys_a.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Texture_QuickSlot_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/QuickSlot_Normal.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Texture_InvenSlotBack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Inventory/SlotBack/InvenSlot_%d.dds"), 5))))
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
		TEXT("../Bin/resources/FBXs/NonAnim/Cube/"), TEXT(".dds"))))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Textures(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/MAP/Field/"), TEXT(".dds"))))
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
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("모델 로드."));
#pragma region Model
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_Component_CModel*/
	XMMATRIX matPretransform = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	if (FAILED(Load_Dirctory_Models(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/NonAnim"), CModel::TYPE_NONANIM, matPretransform)))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Models(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/Anim/Equip"), CModel::TYPE_ANIM, matPretransform)))
		return E_FAIL;
	matPretransform = matPretransform * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(Load_Dirctory_Models(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/Anim/Player"),CModel::TYPE_ANIM,matPretransform)))
		return E_FAIL;

	if (FAILED(Load_Dirctory_Models(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/Mimic"), CModel::TYPE_MIMIC, matPretransform)))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Models(LEVEL_LOADING,
		TEXT("../Bin/resources/FBXs/MAP/Field/"), matPretransform)))
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




#pragma endregion
	lstrcpy(m_szLoadingText, TEXT("객체 로드."));
#pragma region Object
	//ModelObject
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CModelObject::m_szProtoTag,
		CModelObject::Create(m_pDevice, m_pContext))))
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
	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIPanel::m_szProtoTag,
		CUIPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_UIList"),
		CUIListSelector::Create(m_pDevice, m_pContext))))
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
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOADING, CUIInvenSlot::m_szProtoTag,
		CUIInvenSlot::Create(m_pDevice, m_pContext))))
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

#pragma endregion





	lstrcpy(m_szLoadingText, TEXT("로드 완료."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Level_GamePlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스처."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델 로드"));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	XMMATRIX matPretransform = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	matPretransform = matPretransform * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(Load_Dirctory_Models(LEVEL_GAMEPLAY,
		TEXT("../Bin/resources/FBXs/Anim/Boss"), CModel::TYPE_ANIM, matPretransform)))
		return E_FAIL;
	if (FAILED(Load_Dirctory_Models(LEVEL_GAMEPLAY,
		TEXT("../Bin/resources/FBXs/Anim/Monster"), CModel::TYPE_ANIM, matPretransform)))
		return E_FAIL;
	lstrcpy(m_szLoadingText, TEXT("객체 로드"));

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Henesys"),
		CCubeTerrain::Create(m_pDevice, m_pContext, ("../Bin/Resources/Json/Henesys.json")))))
		return E_FAIL;
	/* BAYAR */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Bayar"),
		CBayar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩 완료"));

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
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("UI_Texture_ItemListBack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Home/Home_Dialog_ItemListBack.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("UI_Texture_HighlightBorder"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Home/Home_Dialog_Mat_HighlightBorder.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HOME, TEXT("UI_Texture_SelectedYellow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Home/Home_Dialog_Mat_SelectedYellow.dds"), 1))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("모델 로드."));
	XMMATRIX matPretransform = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
	matPretransform = matPretransform * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(Load_Dirctory_Models(LEVEL_HOME,
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
		CUIItemIndicator::Create(m_pDevice, m_pContext,LEVEL_HOME, TEXT("UI_Texture_HomeDialog")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로드 완료."));

	m_isFinished = true;

	return S_OK;
}


HRESULT CLoader::Load_SkillData()
{
	json j;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/SkillData.json", &j)))
		return E_FAIL;
	for (auto& skill : j["skills"])
	{
		SKILLDB->Insert_Data(skill);
	}
	return S_OK;
}

HRESULT CLoader::Load_Dirctory_Models(LEVELID eLevId,  const _tchar* szDirPath,CModel::TYPE eType ,_fmatrix PreTransformMatrix)
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

		XMMATRIX matGlobal = XMMatrixScaling(1 / 150.0f, 1 / 150.0f, 1 / 150.0f);
		matGlobal = matGlobal * XMMatrixRotationY(XMConvertToRadians(180.f));

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

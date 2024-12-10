#include "..\Public\GameInstance.h"

#include "Renderer.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Input_Device.h"
#include "Controller.h"
#include "UIManager.h"
#include "Light_Manager.h"
#include "Physics.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "Engine_Utility.h"
#include "RenderTarget_Manager.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext, _uint iLayerCouint)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iViewportWidth, EngineDesc.iViewportHeight, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_pLight_Manager = CLight_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;
	m_pPipeLine = CPipeLine::Create(*ppDevice, *ppContext);
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pUIManager = CUIManager::Create();
	if (nullptr == m_pUIManager)
		return E_FAIL;
	m_pController = CController::Create(m_pInput_Device, m_pUIManager);
	if (nullptr == m_pController)
		return E_FAIL;


	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pPhysics = CPhysics::Create(this, EngineDesc.iViewportWidth, EngineDesc.iViewportHeight);
	if (nullptr == m_pPhysics)
		return E_FAIL;

	m_pCollisionManager = CCollisionManager::Create(m_pObject_Manager,iLayerCouint);
	if (nullptr == m_pCollisionManager)
		return E_FAIL;
	m_pEventManager = CEventManager::Create();
	if (nullptr == m_pEventManager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInput_Device->Update_InputDev();
	m_pUIManager->Update();

	m_pController->Update(fTimeDelta);
	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pLevel_Manager->Update(fTimeDelta);
	m_pObject_Manager->Update(fTimeDelta);
	m_pCollisionManager->Update_Collision();
	m_pObject_Manager->Late_Update(fTimeDelta);
	m_pObject_Manager->Final_Update();
	m_pEventManager->DispatchEvent();

	m_pPipeLine->Update();

}

HRESULT CGameInstance::Render_Begin(const _float4 & vClearColor)
{
	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	m_pRenderer->Draw_RenderObject();

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	return m_pGraphic_Device->Present();
}

void CGameInstance::Clear(_int iLevelID)
{
	m_pObject_Manager->Clear(iLevelID);

	m_pPrototype_Manager->Clear(iLevelID);
	m_pUIManager->Clear();
	//m_pLight_Manager->Clear();
}

_float CGameInstance::Get_TimeDelta(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Update_TimeDelta(const _wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	return m_pTimer_Manager->Update_TimeDelta(strTimerTag);
}

TRANSFORM_KEYFRAME CGameInstance::Lerp_Frame(const TRANSFORM_KEYFRAME& Frame1, const TRANSFORM_KEYFRAME& Frame2, _float fRatio)
{
	return CEngineUtility::Lerp_Frame(Frame1, Frame2, fRatio);
}

_float CGameInstance::Get_RandomFloat(_float fMin, _float fMax)
{
	return CEngineUtility::Get_RandomFloat(fMin, fMax);
}

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);
}

HRESULT CGameInstance::Open_Level(_int iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

_int CGameInstance::Get_CurrentLevelID() const
{
	if (nullptr == m_pLevel_Manager)
		return -1;

	return m_pLevel_Manager->Get_CurrentLevelID();
}

CLevel* CGameInstance::Get_CurrentLevel() const
{
	if (nullptr == m_pLevel_Manager)
		return nullptr;
	return m_pLevel_Manager->Get_CurrentLevel();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CBase * pPrototype)
{
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CBase * CGameInstance::Clone_Prototype(Engine::PROTOTYPE eType, _uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(eType, iLevelIndex, strPrototypeTag, pArg);
}

CBase* CGameInstance::Clone_Proto_Object_Stock(const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ, 0, strPrototypeTag, pArg);
}

CBase* CGameInstance::Clone_Proto_Component_Stock(const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, 0, strPrototypeTag, pArg);
}

CBase* CGameInstance::Clone_Proto_Object_Current(const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(PROTOTYPE::PROTO_GAMEOBJ,m_pLevel_Manager->Get_CurrentLevelID(), strPrototypeTag, pArg);
}

CBase* CGameInstance::Clone_Proto_Component_Current(const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(PROTOTYPE::PROTO_COMPONENT, m_pLevel_Manager->Get_CurrentLevelID(), strPrototypeTag, pArg);

}


HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, _uint iLayerId, void* pArg, bool bDontDestroy)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, iLayerId, pArg, bDontDestroy);
}
HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iLevelIndex, _uint iLayerId, CGameObject* pObj, bool bDontDestroy)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iLevelIndex, iLayerId, pObj, bDontDestroy);
}

CGameObject* CGameInstance::Get_FirstGameObject(_uint iLevelIdx, _uint iLayerId)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_FirstGameObject(iLevelIdx, iLayerId);
}

void CGameInstance::Move_DontDestroyObjects(_uint iOldLevel, _uint iNewLevel)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Move_DontDestroyObjects(iOldLevel, iNewLevel);
}


bool CGameInstance::RayCast(_uint iLayerId, const Ray& tRay, RaycastHit* pOut)
{
	if (nullptr == m_pObject_Manager)
		return false;

	return m_pObject_Manager->RayCast(iLayerId, tRay, pOut);
}

bool CGameInstance::RayCast(const Ray& tRay, RaycastHit* pOut)
{
	if (nullptr == m_pObject_Manager)
		return false;

	return m_pObject_Manager->RayCast(tRay, pOut);
}

list<CGameObject*>* CGameInstance::Get_GameObjectList(_uint iLayerId)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObjectList(iLayerId);
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	return m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4_Inverse(eState);
}

_matrix CGameInstance::Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix_Inverse(eState);
}

const _float4* CGameInstance::Get_CamPosition()
{
	return m_pPipeLine->Get_CamPosition();
}


const KEY_STATE& CGameInstance::GetKeyState(KEY _eKEY)
{
	return m_pInput_Device->GetKeyState(_eKEY);
}

const KEY_STATE& CGameInstance::GetMouseKeyState(MOUSE_KEY _eMouse)
{
	return m_pInput_Device->GetMouseKeyState(_eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSE_MOVE eMouseMove)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseMove);
}

POINT CGameInstance::Get_MousePos()
{
	return m_pInput_Device->Get_MousePos();
}

void CGameInstance::Register_UIObject(CUIObject* pUIObject)
{
	m_pUIManager->Register_UIObject(pUIObject);
}

void CGameInstance::Register_DontDestroy_UIObject(CUIObject* pUIObject)
{
	m_pUIManager->Register_DeontDestroy_UIObject(pUIObject);
}


HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(LightDesc);
}

const LIGHT_DESC* CGameInstance::Get_LightDesc(_uint iIndex) const
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render(pShader, pVIBuffer);
}

void CGameInstance::Possess(CPawn* pPawn)
{
	if (nullptr == m_pController)
		return;

	m_pController->Possess(pPawn);
}

void CGameInstance::Push_Event(CEvent* _event)
{
	if (nullptr == m_pEventManager)
		return;
	m_pEventManager->PushEvent(_event);
}

void CGameInstance::Register_PreEventCallback(_uint _eventType, IEventHandlerWrapperInterface* _handler)
{
	if (nullptr == m_pEventManager)
		return;
	m_pEventManager->RegisterPreEventCallback(_eventType, _handler);
}

void CGameInstance::UnRegister_PreEventCallback(_uint _eventType, const void* _handlerAddress)
{
	if (nullptr == m_pEventManager)
		return;
	m_pEventManager->UnRegisterPreEventCallback(_eventType, _handlerAddress);
}

void CGameInstance::Register_PostEventCallback(_uint _eventType, IEventHandlerWrapperInterface* _handler)
{
	if (nullptr == m_pEventManager)
		return;
	m_pEventManager->RegisterPostEventCallback(_eventType, _handler);

}

void CGameInstance::UnRegister_PostEventCallback(_uint _eventType, const void* _handlerAddress)
{
	if (nullptr == m_pEventManager)
		return;
	m_pEventManager->UnRegisterPostEventCallback(_eventType, _handlerAddress);
}

void CGameInstance::Trigger_Event(CEvent* _event)
{
	if (nullptr == m_pEventManager)
		return;
	m_pEventManager->TriggerEvent(_event);
}

void CGameInstance::Dispatch_Event()
{
	if (nullptr == m_pEventManager)
		return;
	m_pEventManager->DispatchEvent();
}

void CGameInstance::Set_LayerCount(_uint iLayerCount)
{
	if (nullptr == m_pCollisionManager)
		return;

	m_pCollisionManager->Set_LayerCount(iLayerCount);
}

void CGameInstance::Set_CollisionMatrix(_uint eObjectLayer, _uint eSubjectLayer, bool bValue)
{
	if (nullptr == m_pCollisionManager)
		return;

	m_pCollisionManager->Set_CollisionMatrix(eObjectLayer, eSubjectLayer, bValue);
}

void CGameInstance::Zero_CollisionMatrix()
{
	if (nullptr == m_pCollisionManager)
		return;

	m_pCollisionManager->Zero_CollisionMatrix();
}

HRESULT CGameInstance::Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RT_ShaderResource(CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Bind_ShaderResource(pShader, pConstantName, strTargetTag);
}
#ifdef _DEBUG
HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}
#endif

void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->Free();

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	__super::Free();

	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pController);
	Safe_Release(m_pUIManager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPhysics);
	Safe_Release(m_pCollisionManager);
	Safe_Release(m_pEventManager);

}

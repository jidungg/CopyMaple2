#pragma once

#include "Renderer.h"
#include "Prototype_Manager.h"
#include "PipeLine.h"

BEGIN(Engine)
class CColliderBase;
class CPawn;
class CEvent;
class IEventHandlerWrapperInterface;
class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext, _uint iLayerCount);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Begin(const _float4& vClearColor = _float4(0.f, 0.f, 1.f, 1.f));
	HRESULT Draw();
	HRESULT Render_End();
	void Clear(_int iLevelID);

public: /* For.timer_Manager */
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void Update_TimeDelta(const _wstring& strTimerTag);

public://For Utility
	TRANSFORM_KEYFRAME Lerp_Frame(const TRANSFORM_KEYFRAME& Frame1, const TRANSFORM_KEYFRAME& Frame2, _float fRatio);
	_float Get_RandomFloat(_float fMin, _float fMax);
public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);

public: /* for.Level_Manager */
	HRESULT Open_Level(_int iLevelIndex, class CLevel* pNewLevel);
	_int Get_CurrentLevelID()const;
	CLevel* Get_CurrentLevel() const;
public: /* For.Prototype_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(Engine::PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	class CBase* Clone_Proto_Object_Stock(const _wstring& strPrototypeTag, void* pArg = nullptr);
	class CBase* Clone_Proto_Component_Stock(const _wstring& strPrototypeTag, void* pArg = nullptr);
	class CBase* Clone_Proto_Object_Current(const _wstring& strPrototypeTag, void* pArg);
	class CBase* Clone_Proto_Component_Current(const _wstring& strPrototypeTag, void* pArg);

public: /* For.Object_Manager */
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, _uint iLayerId,  void* pArg = nullptr, bool bDontDestroy = false);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, _uint iLayerId, CGameObject* pObj, bool bDontDestroy = false);
	CGameObject* Get_FirstGameObject(_uint iLevIdx, _uint iLayerId);
	void Move_DontDestroyObjects(_uint iOldLevel, _uint iNewLevel);
	bool RayCast(_uint iLayerId, const Ray& tRay, RaycastHit* pOut);
	bool RayCast(const Ray& tRay, RaycastHit* pOut);
	list<CGameObject*>* Get_GameObjectList(_uint iLayerId);

public: /* For.PipeLine */
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState);
	_matrix Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState);
	const _float4* Get_CamPosition();

public: //For Key Manager
	const KEY_STATE& GetKeyState(KEY _eKEY);
	const KEY_STATE& GetMouseKeyState(MOUSE_KEY _eMouse);
	_long	Get_DIMouseMove(MOUSE_MOVE eMouseMove);
	POINT Get_MousePos();

public://For UI Manager
	void Register_UIObject(class CUIObject* pUIObject);
	void Register_DontDestroy_UIObject(class CUIObject* pUIObject);

public: /* Light_Manager */
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	const LIGHT_DESC* Get_LightDesc(_uint iIndex) const;
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);


public://For Controller
	void Possess(CPawn* pPawn);

public://For EventManager
	void Push_Event(CEvent* _event);
	void Register_PreEventCallback(_uint _eventType, IEventHandlerWrapperInterface* _handler);
	void UnRegister_PreEventCallback(_uint _eventType, const void* _handlerAddress);
	void Register_PostEventCallback(_uint _eventType, IEventHandlerWrapperInterface* _handler);
	void UnRegister_PostEventCallback(_uint _eventType, const void* _handlerAddress);
	void Trigger_Event(CEvent* _event);
	void Dispatch_Event();

public://For CollisionManager
	void Set_LayerCount(_uint iLayerCount);
	void Set_CollisionMatrix(_uint eObjectLayer, _uint eSubjectLayer, bool bValue);
	void Zero_CollisionMatrix();

public: // Target_Manager 
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag);
	HRESULT End_MRT();
	HRESULT Bind_RT_ShaderResource(class CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag);

public: /* Frustum */
	_bool Frustum_Culling_World(_fvector vWorldPos, _float fRange = 0.f);

public: //FONT
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	class CCustomFont* Find_Font(const _wstring& strFontTag);
	HRESULT Render_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f));


#ifdef _DEBUG
public:
	HRESULT Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);


#endif

private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CInput_Device*				m_pInput_Device = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };	
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*			m_pPrototype_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CPipeLine*					m_pPipeLine = { nullptr };
	class CRenderer*					m_pRenderer = { nullptr };
	class CController*					m_pController = { nullptr };
	class CUIManager*					m_pUIManager = { nullptr };
	class CLight_Manager*				m_pLight_Manager = { nullptr };
	class CPhysics*						m_pPhysics = { nullptr };
	class CEventManager*			m_pEventManager = { nullptr };
	class CCollisionManager* m_pCollisionManager = { nullptr };
	class CTarget_Manager* m_pTarget_Manager = { nullptr };
	class CCollider_Frustum* m_pFrustum = { nullptr };
	class CFontManager* m_pFontManager = { nullptr };
public:
	static void Release_Engine();

public:
	virtual void Free() override;
};

END
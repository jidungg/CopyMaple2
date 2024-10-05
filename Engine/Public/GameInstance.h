#pragma once

#include "Renderer.h"
#include "Prototype_Manager.h"
#include "PipeLine.h"

/* 1. Ŭ���̾�Ʈ �����ڰ� ����� ���� �������� �������ִ� ��ɵ��� �����ش� .*/
/* 2. ������ ����� ��Ȱ�ϰ� ����� �� �ֵ��� ������ �ʱ�ȭ�Ѵ�.Initialize_Engine */
/* 3. ��� ����ÿ� ���� ��� ������ ���ؼ� �Ҵ��ߴ� ��� �����͸� �����Ѵ�. Release_Engine*/
/* 4. �������� ������ �ʿ��� ��� ��ü�� �Ѱ��� ��Ƽ� ������ �ش�. */

BEGIN(Engine)
class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Begin(const _float4& vClearColor = _float4(0.f, 0.f, 1.f, 1.f));
	HRESULT Draw();
	HRESULT Render_End();
	void Clear(_int iLevelID);

public: /* For.timer_Manager */
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void Update_TimeDelta(const _wstring& strTimerTag);

public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);

public: /* for.Level_Manager */
	HRESULT Open_Level(_int iLevelIndex, class CLevel* pNewLevel);

public: /* For.Prototype_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(Engine::PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	class CBase* Clone_Stock_Proto_Object(const _wstring& strPrototypeTag, void* pArg = nullptr);
	class CBase* Clone_Stock_Proto_Component(const _wstring& strPrototypeTag, void* pArg = nullptr);

public: /* For.Object_Manager */
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	HRESULT Add_GameObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* pObj);


public: /* For.PipeLine */
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState);

public: //For Key Manager
	const KEY_STATE& GetKeyState(KEY _eKEY);
	const KEY_STATE& GetMouseKeyState(MOUSE_KEY _eMouse);
	_long	Get_DIMouseMove(MOUSE_MOVE eMouseMove);

private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CInput_Device*				m_pInput_Device = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };	
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*			m_pPrototype_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CPipeLine* m_pPipeLine = { nullptr };
	class CRenderer*					m_pRenderer = { nullptr };
public:
	static void Release_Engine();

public:
	virtual void Free() override;
};

END
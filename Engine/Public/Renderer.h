#pragma once

#include "Base.h"

BEGIN(Engine)

class  CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_NONLIGHT,RG_BLEND, RG_UI, RG_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw_RenderObject();

private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };
	list<class CGameObject*>			m_RenderObjects[RG_END];
	class CGameInstance* m_pGameInstance = { nullptr };
	class CShader* m_pShader = { nullptr };
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	_float4x4							m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};

#ifdef _DEBUG
private:
	list<class CComponent*>				m_DebugComponents;

#endif
private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Lights();
	HRESULT Render_Final();
	HRESULT Render_NonLight();
	HRESULT Render_Blend();
	HRESULT Render_UI();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
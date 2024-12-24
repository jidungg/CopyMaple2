#pragma once
#include "UIObject.h"

BEGIN(Engine)
class CRenderTarget;
class CShader;
class CVIBuffer_Rect;
END
BEGIN(Client)

class CUIModelPad :
    public CUIObject
{

public:
	typedef struct UIModelPadDesc : public CUIObject::UIOBJECT_DESC
	{
		CGameObject* pModel = { nullptr };
	}UIMODELPAD_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIModelPad";
public:
	explicit CUIModelPad(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIModelPad(const CUIModelPad& Prototype);
	virtual ~CUIModelPad() = default;


public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Bind_ShaderResources();
	HRESULT Ready_DepthStencilView(_uint iWinCX, _uint iWinCY);
private:
	CGameObject* m_pModel = { nullptr };
	CShader* m_pModelShaderCom = { nullptr };
	_matrix	 m_ModelViewMatrix{}, m_ModelProjMatrix{};
	D3D11_VIEWPORT m_tModelViewport = {};
	D3D11_VIEWPORT m_tOriginalViewport = {};

	CRenderTarget* m_pModelRenderTarget = { nullptr };
	ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
public:
	static CUIModelPad* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
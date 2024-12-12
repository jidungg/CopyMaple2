#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Client)
class CUIPanel :
    public CUIObject
{
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Panel";
public:
	typedef struct PanelDesc: public CUIObject::UIOBJECT_DESC
	{
		CTexture* pTextureCom = { nullptr };
		_float4  vBorder = { 0,0,0,0 };
	}PANEL_DESC;

protected:
	CUIPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPanel(const CUIPanel& Prototype);
	virtual ~CUIPanel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;

	void Set_Texture(CTexture* pTexture);
	bool Is_TextureNullptr() { return m_pTextureCom == nullptr; }
protected:
	virtual HRESULT Bind_ShaderResources();
private:
	HRESULT Ready_Components();

protected:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_float4 m_vBorder = { 0,0,0,0 };//상 하 좌 우 순 픽셀 수
public:
	static CUIPanel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Client)
class CUIPanel :
    public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		LEVELID eLevelID = LEVEL_END;
		const _tchar* szTextureTag = nullptr;
	}PANEL_DESC;

protected:
	CUIPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPanel(const CUIPanel& Prototype);
	virtual ~CUIPanel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual HRESULT Render_Self() override;




private:
	HRESULT Ready_Components(LEVELID eLevelID,const _tchar* szTextureTag);
	HRESULT Bind_ShaderResources();

public:
	static CUIPanel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
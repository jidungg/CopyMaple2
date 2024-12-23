#pragma once
#include "UIObject.h"

BEGIN(Client)
//CheckMouseOver 검사 시 자식오브젝트가 대신 검사받음.
class CUIContainer :
    public CUIObject
{
protected:
	explicit CUIContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUIContainer(const CUIContainer& Prototype);
	virtual ~CUIContainer() = default;

public:
	virtual bool Check_MouseOver(POINT fPos);
	virtual HRESULT Render() override;
private:

public:
	static CUIContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
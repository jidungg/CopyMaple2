#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Client)
class CCanvas :
    public CUIObject
{
public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		
	}CANVAS_DESC;
private:
	CCanvas();
	CCanvas(const CCanvas& rhs);
	virtual ~CCanvas() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render_Self() override;

public:
	static CCanvas* Create();
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END


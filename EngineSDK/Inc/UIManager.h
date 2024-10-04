#pragma once
#include "Engine_Defines.h"
#include "UIObject.h"

BEGIN(Engine)

class CUIManager :
    public CBase
{
private:
	CUIManager(void);
	virtual ~CUIManager(void) = default;

public:
	HRESULT Initialize();
private:
	CUIObject*		m_pCanvas;

public:
	static CUIManager* Create();
	virtual void Free(void) override;
};

END
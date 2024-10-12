#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CInput_Device;
class CUIManager;
class CPipeLine;
class CController :
    public CBase
{
private:
	CController(CInput_Device* pInput, CUIManager* pUIManager);
	virtual ~CController(void) = default;


public:
	HRESULT Initialize();
	void Update();
private:
	CInput_Device*	m_pInput_Device = nullptr;
	CUIManager*		m_pUIManager = nullptr;

public:
	static CController* Create(CInput_Device* pInput, CUIManager* pUIManager);
	virtual void Free(void) override;

};

END
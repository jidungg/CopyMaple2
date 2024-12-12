#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CInput_Device;
class CUIManager;
class CPipeLine;
class CPawn;
class CController :
    public CBase
{
private:
	CController(CInput_Device* pInput, CUIManager* pUIManager);
	virtual ~CController(void) = default;


public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	void Possess(CPawn* pPawn);
	void Clear();
private:
	CInput_Device*	m_pInput_Device = nullptr;
	CUIManager*		m_pUIManager = nullptr;
	CPawn*				m_pCurrentPawn = nullptr;
public:
	static CController* Create(CInput_Device* pInput, CUIManager* pUIManager);
	virtual void Free(void) override;

};

END
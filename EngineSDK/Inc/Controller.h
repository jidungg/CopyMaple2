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
	CController(CInput_Device* pInput, CUIManager* pUIManager, _uint iWinSizeX, _uint iWinSizeY, CPipeLine* pPipeLine);
	virtual ~CController(void) = default;


public:
	HRESULT Initialize();
	void Update();
private:
	CInput_Device*	m_pInput_Device = nullptr;
	CUIManager*		m_pUIManager = nullptr;
	_uint m_iWinSizeX = 0;
	_uint m_iWinSizeY = 0;
	CPipeLine* m_pPipeLine = nullptr;
public:
	static CController* Create(CInput_Device* pInput, CUIManager* pUIManager, _uint iWinSizeX, _uint iWInSizeY, CPipeLine* pPipeLine);
	virtual void Free(void) override;

};

END
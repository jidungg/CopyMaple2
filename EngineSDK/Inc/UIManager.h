#pragma once
#include "Engine_Defines.h"
#include "UIObject.h"


BEGIN(Engine)

class CController;
class CGameInstance;
class CUIManager :
    public CBase
{
private:
	CUIManager(void);
	virtual ~CUIManager(void) = default;

public:
	HRESULT Initialize();
	void Update();

public:
	void Register_UIObject(CUIObject* pUIObject);
	void Clear();

	bool Consume_MouseLButtonDown();
	bool Consume_MouseLButtonUp();
	bool Consume_MouseRButtonDown();
	bool Consume_MouseRButtonUp();
private:
	list<CUIObject*> m_UIObjectList;
	CGameInstance* m_pGameInstance = nullptr;
	CUIObject* m_pFocusedUI = nullptr;
	CUIObject* m_pPressedUI = nullptr;
	CUIObject* m_pRightPressedUI = nullptr;
public:
	static CUIManager* Create();
	virtual void Free(void) override;
};

END
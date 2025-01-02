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
	void Register_DontDestroy_UIObject(CUIObject* pUIObject);
	void Get_MouseOverObjects(const POINT& tMousePoint, list<CUIObject*>& listOut);
	void Clear();

	bool Consume_MouseLButtonDown(const POINT& tMousePoint);
	bool Consume_MouseLButtonUp(const POINT& tMousePoint);
	bool Consume_MouseRButtonDown();
	bool Consume_MouseRButtonUp();
	bool Consume_MouseMove(const POINT& tMousePoint,const DIMOUSESTATE& tState);

private:
	list<CUIObject*> m_UIObjectList;
	list<CUIObject*> m_DontDestroyUIObjectList;
	CGameInstance* m_pGameInstance = nullptr;
	CUIObject* m_pFocusedUI = nullptr;
	CUIObject* m_pPressedUI = nullptr;
	CUIObject* m_pRightPressedUI = nullptr;
	CUIObject* m_pDraggedUI = nullptr;
public:
	static CUIManager* Create();
	virtual void Free(void) override;
};

END
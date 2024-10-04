#pragma once
#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)


// 키 매니저
class ENGINE_DLL CInput_Device : public CBase
{

private:
	explicit CInput_Device();
	virtual ~CInput_Device();
public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void Update_InputDev();

public:
	// Get
	const KEY_STATE& GetKeyState(KEY _eKEY) // 키입력 동기화 o 
	{
		return m_vecKey[(int)_eKEY].eState;
	}
	const KEY_STATE& GetMouseKeyState(MOUSE_KEY _eMouse)
	{
		return m_arrMouseKey[(int)_eMouse].eState;
	}
	_long	Get_DIMouseMove(MOUSE_MOVE eMouseMove)
	{
		return *(((_long*)&m_tMouseState) + (int)eMouseMove);
	}
	pair<LONG, LONG> Get_MousePos()
	{
		return { m_lMouseX, m_lMouseY };
	}
private:

private:
	vector<tKeyInfo> m_vecKey; // 키 인포 타입을 저장 할 가변배열. // 벡터의 인덱스번호가 해당 키값.
private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
	_byte					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수

	
	tKeyInfo				m_arrMouseKey[(int)MOUSE_KEY::LAST];
	DIMOUSESTATE			m_tMouseState;
	LONG					m_lMouseX = 0;
	LONG					m_lMouseY = 0;
public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void Free(void) override;
};
END

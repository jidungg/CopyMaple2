#pragma once
#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)


// Ű �Ŵ���
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
	const KEY_STATE& GetKeyState(KEY _eKEY) // Ű�Է� ����ȭ o 
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
	vector<tKeyInfo> m_vecKey; // Ű ���� Ÿ���� ���� �� �����迭. // ������ �ε�����ȣ�� �ش� Ű��.
private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
	_byte					m_byKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����

	
	tKeyInfo				m_arrMouseKey[(int)MOUSE_KEY::LAST];
	DIMOUSESTATE			m_tMouseState;
	LONG					m_lMouseX = 0;
	LONG					m_lMouseY = 0;
public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void Free(void) override;
};
END

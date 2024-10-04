#pragma once
#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)

enum class KEY_STATE
{
	NONE,// �������� ������ �ʾҰ�, ���絵 ������ ����.
	DOWN, // �� ���� ����.
	PRESSING, // ������ �ִ�.
	UP, // �� �� ����.
};

enum class MOUSE_KEY { LB, RB, MB, LAST, };
enum class MOUSE_MOVE { X, Y, Z, LAST, };

enum class KEY // ��ü������ ���� �� Ű ����
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,
	NUM0,
	NUMPAD8,
	NUMPAD4,
	NUMPAD5,
	NUMPAD6,
	NUMPAD2,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	H,
	J,
	K,
	M,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,
	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,
	TAB,
	LAST, // ���� �˸���.
};

// Ű ������ ���� ����ü
struct tKeyInfo
{
	KEY_STATE   eState; // Ű�� ���°�
	bool		bPrevPush;	// ���� �����ӿ��� ���ȴ��� ����.
};

// Ű �Ŵ���
class ENGINE_DLL CKeyMgr : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	explicit CKeyMgr();
	virtual ~CKeyMgr();
public:
	HRESULT Ready_KeyMgr(HINSTANCE hInst, HWND hWnd);
	void Update_KeyMgr();
private:
	void Check_KeyBoard();
	void Check_Mouse();
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
private:
	virtual void		Free();
};
END

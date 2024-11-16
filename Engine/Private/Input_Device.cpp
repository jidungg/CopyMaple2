#include "Input_Device.h"


int g_ArrDK[(int)KEY::LAST] = {
    DIK_LEFT,
    DIK_RIGHT,
    DIK_UP,
    DIK_DOWN,
    DIK_0,
    DIK_1,
    DIK_2,
    DIK_3,
    DIK_4,
    DIK_5,
    DIK_6,
    DIK_7,
    DIK_8,
    DIK_9,
    DIK_NUMPAD8,
    DIK_NUMPAD4,
    DIK_NUMPAD5,
    DIK_NUMPAD6,
    DIK_NUMPAD2,
    DIK_Q,
    DIK_W,
    DIK_E,
    DIK_R,
    DIK_T,
    DIK_Y,
    DIK_U, // 
    DIK_H, // �߰�
    DIK_J, // �߰�
    DIK_K, // �߰�
    DIK_M, // �߰�
    DIK_I,
    DIK_O,
    DIK_P,
    DIK_A,
    DIK_S,
    DIK_D,
    DIK_F,
    DIK_G,
    DIK_Z,
    DIK_X,
    DIK_C,
    DIK_V,
    DIK_B,
    DIK_LALT,     // ALT,
    DIK_LCONTROL, // CTRL,
    DIK_LSHIFT,   // LSHIFT,
    DIK_SPACE,    // SPACE,
    DIK_RETURN,   // ENTER,
    DIK_ESCAPE,   // ESC,
    DIK_TAB,       // TAB,
    DIK_F1,
	DIK_F2,
	DIK_F3,
	DIK_F4,
	DIK_F5,
	DIK_F6,
	DIK_F7,
	DIK_F8,
	DIK_F9,
	DIK_F10,
	DIK_F11,
	DIK_F12,
};


CInput_Device::CInput_Device() : m_vecKey{} {}

CInput_Device::~CInput_Device() { Free(); }

HRESULT CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{
	m_hInst = hInst;
	m_hWnd = hWnd;

    if(FAILED( DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;
    

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;
    m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

    m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    m_pKeyBoard->Acquire();

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

    m_pMouse->SetDataFormat(&c_dfDIMouse);

    m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    m_pMouse->Acquire();

    for (int i = 0; i < (int)KEY::LAST; ++i)
    {
        m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
    }

    for (int i = 0; i < (int)MOUSE_KEY::LAST; ++i)
    {
        m_arrMouseKey[i] = { tKeyInfo{ KEY_STATE::NONE, false } };
    }
    return S_OK;
}

void CInput_Device::Update_InputDev()
{
    // Direct Input StateCheck
    m_pKeyBoard->GetDeviceState(256, m_byKeyState);
    m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
    GetCursorPos(&m_tMousePos);
    ScreenToClient(m_hWnd, &m_tMousePos);


    HWND hWnd = GetFocus(); 

    if (nullptr != hWnd) 
    {
        for (int i = 0; i < (int)KEY::LAST; ++i)
        {
          
            if (m_byKeyState[g_ArrDK[i]] & 0x80)
            {
                if (m_vecKey[i].bPrevPush)
                {
                    m_vecKey[i].eState = KEY_STATE::PRESSING;
                }
                else
                {
                    m_vecKey[i].eState = KEY_STATE::DOWN;
                }
                m_vecKey[i].bPrevPush = true;
            }
            else
            {
                if (m_vecKey[i].bPrevPush)
                {
                    m_vecKey[i].eState = KEY_STATE::UP;
                }
                else
                {
                    m_vecKey[i].eState = KEY_STATE::NONE;
                }
                m_vecKey[i].bPrevPush = false;
            }
        }

        // Mouse
        for (int i = 0; i < (int)MOUSE_KEY::LAST; ++i)
        {
            if (m_tMouseState.rgbButtons[i]) 
            {
                if (m_arrMouseKey[i].bPrevPush)
                {
                    m_arrMouseKey[i].eState = KEY_STATE::PRESSING;
                }
                else 
                {
                    m_arrMouseKey[i].eState = KEY_STATE::DOWN;
                }
                m_arrMouseKey[i].bPrevPush = true;
            }
            else 
            {
                if (m_arrMouseKey[i].bPrevPush)
                {
                    m_arrMouseKey[i].eState = KEY_STATE::UP;
                }
                else 
                {
                    m_arrMouseKey[i].eState = KEY_STATE::NONE;
                }
                m_arrMouseKey[i].bPrevPush = false;
            }
        }


    }
    else 
    {
        for (int i = 0; i < (int)KEY::LAST; ++i)
        {
            m_vecKey[i].bPrevPush = false;

            if (m_vecKey[i].eState == KEY_STATE::DOWN || m_vecKey[i].eState == KEY_STATE::PRESSING)
            {
                m_vecKey[i].eState = KEY_STATE::UP;
            }
            else if (m_vecKey[i].eState == KEY_STATE::UP)
            {
                m_vecKey[i].eState = KEY_STATE::NONE;
            }
        }

        for (int i = 0; i < (int)MOUSE_KEY::LAST; ++i)
        {
            m_arrMouseKey[i].bPrevPush = false;

            if (m_arrMouseKey[i].eState == KEY_STATE::DOWN || m_arrMouseKey[i].eState == KEY_STATE::PRESSING)
            {
                m_arrMouseKey[i].eState = KEY_STATE::UP;
            }
            else if (m_arrMouseKey[i].eState == KEY_STATE::UP)
            {
                m_arrMouseKey[i].eState = KEY_STATE::NONE;
            }
        }
    }
}

CInput_Device* CInput_Device::Create(HINSTANCE hInst, HWND hWnd)
{
    CInput_Device* pInstance = new CInput_Device();

    if (FAILED(pInstance->Initialize(hInst, hWnd)))
    {
        MSG_BOX("Failed to Created : CRenderer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInput_Device::Free()
{
    __super::Free();

    Safe_Release(m_pKeyBoard);
    Safe_Release(m_pMouse);
    Safe_Release(m_pInputSDK);
}

#include "Controller.h"
#include "UIManager.h"
#include "Input_Device.h"

CController::CController(CInput_Device* pInput, CUIManager* pUIManager)
	: m_pInput_Device(pInput)
	, m_pUIManager(pUIManager)
{
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pUIManager);
}

HRESULT CController::Initialize()
{
	return S_OK;
}

void CController::Update()
{
	POINT tPosition = m_pInput_Device->Get_MousePos();
	if (m_pInput_Device->GetMouseKeyState(MOUSE_KEY::LB) == KEY_STATE::DOWN)
	{
  		m_pUIManager->Consume_MouseLButtonDown();
	}
	else if (m_pInput_Device->GetMouseKeyState(MOUSE_KEY::LB) == KEY_STATE::UP)
	{
		m_pUIManager->Consume_MouseLButtonUp();
	}
	if (m_pInput_Device->GetMouseKeyState(MOUSE_KEY::RB) == KEY_STATE::DOWN)
	{
		m_pUIManager->Consume_MouseRButtonDown();
	}
	else if (m_pInput_Device->GetMouseKeyState(MOUSE_KEY::RB) == KEY_STATE::UP)
	{
		m_pUIManager->Consume_MouseRButtonUp();
	}



}

CController* CController::Create(CInput_Device* pInput, CUIManager* pUIManager)
{
    CController* pInstance = new CController(pInput, pUIManager);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CRenderer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CController::Free(void)
{
	__super::Free();
}

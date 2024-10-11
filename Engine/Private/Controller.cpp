#include "Controller.h"
#include "UIManager.h"
#include "Input_Device.h"
#include "Physics.h"
#include "PipeLine.h"

CController::CController(CInput_Device* pInput, CUIManager* pUIManager, _uint iWinSizeX, _uint iWinSizeY, CPipeLine* pPipeLine)
	: m_pInput_Device(pInput)
	, m_pUIManager(pUIManager)
	,m_iWinSizeX(iWinSizeX),
	m_iWinSizeY(iWinSizeY)
	, m_pPipeLine(pPipeLine)
{
	Safe_AddRef(m_pInput_Device);
	//Safe_AddRef(m_pUIManager);
	//Safe_AddRef(m_pPipeLine);
}

HRESULT CController::Initialize()
{
	return S_OK;
}

void CController::Update()
{
	POINT tPosition = m_pInput_Device->Get_MousePos();

	if (m_pInput_Device->GetMouseKeyState(MOUSE_KEY::LB) == KEY_STATE::DOWN)
		 m_pUIManager->Consume_MouseLButtonDown();
	else if (m_pInput_Device->GetMouseKeyState(MOUSE_KEY::LB) == KEY_STATE::UP)
	{
		if (false == m_pUIManager->Consume_MouseLButtonUp())
		{
			//TODO : Ray 만들어서  
			Ray ray{ POINT{0,0}, tPosition, m_iWinSizeX, m_iWinSizeY, 
				m_pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW),
				m_pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)};
 			_float fDist = 0.f;
		}
	}

	if (m_pInput_Device->GetMouseKeyState(MOUSE_KEY::RB) == KEY_STATE::DOWN)
		m_pUIManager->Consume_MouseRButtonDown();
	else if (m_pInput_Device->GetMouseKeyState(MOUSE_KEY::RB) == KEY_STATE::UP)
		m_pUIManager->Consume_MouseRButtonUp();

}


CController* CController::Create(CInput_Device* pInput, CUIManager* pUIManager, _uint iWinSizeX, _uint iWInSizeY, CPipeLine* pPipeLine)
{
	CController* pInstance = new CController(pInput, pUIManager, iWinSizeX, iWInSizeY,pPipeLine);

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

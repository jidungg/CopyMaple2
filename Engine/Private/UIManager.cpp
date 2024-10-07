#include "UIManager.h"

#include "GameInstance.h"

CUIManager::CUIManager(void)
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
}

HRESULT CUIManager::Initialize()
{
    return S_OK;
}

void CUIManager::Update()
{
    POINT tPosition = m_pGameInstance->Get_MousePos();
	CUIObject* pTopUI = nullptr;
    for (auto& pUIObject : m_UIObjectList)
    {
        if (false == pUIObject->Is_Active()) continue;
        if (pUIObject->Check_MouseOver(tPosition))
        {
            if (pTopUI == nullptr)
                pTopUI = pUIObject;
            else
				pTopUI = pUIObject->Get_Priority() > pTopUI->Get_Priority() ? pUIObject : pTopUI;
        }
    }
    if (pTopUI == nullptr)
    {
		m_pFocusedUI = nullptr;
        return;
    }
    m_pFocusedUI = pTopUI->Find_FocusedUI(tPosition);
	if (m_pFocusedUI != nullptr)
        m_pFocusedUI->On_MouseOver();
}
void CUIManager::Register_UIObject(CUIObject* pUIObject)
{
	m_UIObjectList.push_back(pUIObject);
	Safe_AddRef(pUIObject);
}
void CUIManager::Clear()
{
	for (auto& pUIObject : m_UIObjectList)
		Safe_Release(pUIObject);
	m_UIObjectList.clear();
}
bool CUIManager::Consume_MouseLButtonDown()
{
    if (m_pFocusedUI != nullptr)
    {
		m_pPressedUI = m_pFocusedUI;
        m_pPressedUI->Increase_Priority();
        return m_pPressedUI->Consume_MouseLButtonDown();
    }
    else
    {
        m_pPressedUI = nullptr;
        return false;
    }
}

bool CUIManager::Consume_MouseLButtonUp()
{
	bool bConsume = false;
    if (m_pFocusedUI != nullptr)
    {
        bConsume = bConsume || m_pFocusedUI->Consume_MouseLButtonUp();
        if (m_pPressedUI == m_pFocusedUI)
            bConsume = bConsume || m_pPressedUI->Consume_MouseClick();
    }
	m_pPressedUI = nullptr;
    return bConsume;
}

bool CUIManager::Consume_MouseRButtonDown()
{
    if (m_pFocusedUI != nullptr)
        return m_pFocusedUI->Consume_MouseRButtonDown();
    return false;
}

bool CUIManager::Consume_MouseRButtonUp()
{
    if (m_pFocusedUI != nullptr)
        return m_pFocusedUI->Consume_MouseRButtonUp();
    return false;
}


CUIManager* CUIManager::Create()
{
    CUIManager* pInstance = new CUIManager();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CRenderer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUIManager::Free(void)
{
	__super::Free();
    Clear();
	Safe_Release(m_pGameInstance);
}

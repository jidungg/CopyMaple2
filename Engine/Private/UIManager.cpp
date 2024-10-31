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
    for (auto& pUIObject : m_DontDestroyUIObjectList)
    {
        if (false == pUIObject->Is_Active()) continue;
        if (pUIObject->Check_MouseOver(tPosition))
        {
            if (pTopUI == nullptr)
                pTopUI = pUIObject;
            else
                pTopUI = pUIObject->Get_Priority() > pTopUI->Get_Priority() ? pUIObject : pTopUI;
        }
        else
            pUIObject->MouseNotOver();
    }
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
        else
			pUIObject->MouseNotOver();
    }
    if (pTopUI == nullptr)
    {
		m_pFocusedUI = nullptr;
        return;
    }
    m_pFocusedUI = pTopUI->Find_FocusedUI(tPosition);
	//if (m_pFocusedUI != nullptr)
 //       m_pFocusedUI->MouseOver();
	//pTopUI->MouseNotOver(m_pFocusedUI);
}
void CUIManager::Register_UIObject(CUIObject* pUIObject)
{
    if(pUIObject->Is_DontDestroy())
		Register_DeontDestroy_UIObject(pUIObject);
	else
    {
        m_UIObjectList.push_back(pUIObject);
        Safe_AddRef(pUIObject);
    }

}
void CUIManager::Register_DeontDestroy_UIObject(CUIObject* pUIObject)
{
	m_DontDestroyUIObjectList.push_back(pUIObject);
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
        m_pPressedUI->On_MouseLButtonDown();
        return true;
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

    if (m_pPressedUI == m_pFocusedUI&& m_pFocusedUI != nullptr)
    {
        m_pPressedUI->On_MouseClick();
        bConsume = true;
    }
	else if (m_pPressedUI != nullptr)
		m_pPressedUI->On_MouseLButtonUp();
    for (auto& pUI : m_UIObjectList)
    {
		if (pUI == m_pPressedUI) continue;
        pUI->On_MouseLButtonUp();
    }
	m_pPressedUI = nullptr;
    return bConsume;
}

bool CUIManager::Consume_MouseRButtonDown()
{
    if (m_pFocusedUI != nullptr)
    {
        m_pRightPressedUI = m_pFocusedUI;
        m_pRightPressedUI->Increase_Priority();
        m_pRightPressedUI->On_MouseRButtonDown();
        return true;
    }
    else
    {
        m_pRightPressedUI = nullptr;
        return false;
    }
}

bool CUIManager::Consume_MouseRButtonUp()
{
    bool bConsume = false;
    if (m_pFocusedUI != nullptr)
    {
        if (m_pRightPressedUI == m_pFocusedUI)
        {
            m_pRightPressedUI->On_MouseRightClick();
            bConsume = true;
        }
    }
    for (auto& pUI : m_UIObjectList)
    {
        if (pUI == m_pRightPressedUI) continue;
        pUI->On_MouseRButtonUp();
    }
    m_pRightPressedUI = nullptr;
    return bConsume;
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
    for (auto& pUI : m_DontDestroyUIObjectList)
    {
		Safe_Release(pUI);
    }
    m_DontDestroyUIObjectList.clear();
}

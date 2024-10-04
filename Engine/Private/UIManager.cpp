#include "UIManager.h"

CUIManager::CUIManager(void)
{
}

HRESULT CUIManager::Initialize()
{
    return S_OK;
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
	Safe_Release(m_pCanvas);
}

#include "Camera_UI.h"

CCamera_UI::CCamera_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCamera(pDevice, pContext)
{
}

CCamera_UI::CCamera_UI(const CCamera_UI& Prototype)
	: CCamera(Prototype)
{
}

HRESULT CCamera_UI::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CCamera_UI::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

CCamera_UI* CCamera_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CGameObject* CCamera_UI::Clone(void* pArg)
{
    return nullptr;
}

void CCamera_UI::Free()
{
}

#include "Button_Component.h"

CButton_Component::CButton_Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CButton_Component::CButton_Component(const CButton_Component& rhs)
	: CComponent(rhs)
{
}

HRESULT CButton_Component::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton_Component::Initialize(void* pArg)
{
	return S_OK;
}

CButton_Component* CButton_Component::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CComponent* CButton_Component::Clone(void* pArg)
{
	return nullptr;
}

void CButton_Component::Free()
{
}

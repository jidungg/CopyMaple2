#include "Collider.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider& Prototype)
	:CComponent(Prototype)
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{ 

	return S_OK;
}

void CCollider::Late_Update(_float fTimeDelta)
{
}



void CCollider::Free()
{
	__super::Free();
}

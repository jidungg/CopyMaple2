#include "stdafx.h"
#include "MyTransform.h"

CMyTransform::CMyTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTransform(pDevice, pContext)
{
}

CMyTransform::CMyTransform(const CMyTransform& Prototype)
	: CTransform(Prototype)
{
}

void CMyTransform::Go_Direction(DIRECTION eDirection, _float fTimeDelta)
{
	XMVECTOR vDirection;
	switch (eDirection)
	{
	case Client::DIRECTION::XM:
		vDirection = XMVectorSet(-1.f, 0.f, 0.f, 0.f);
		break;
	case Client::DIRECTION::XP:
		vDirection = XMVectorSet(1.f, 0.f, 0.f, 0.f);
		break;
	case Client::DIRECTION::ZM:
		vDirection = XMVectorSet(0.f, 0.f, -1.f, 0.f);
		break;
	case Client::DIRECTION::ZP:
		vDirection = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		break;
	case Client::DIRECTION::XMZP:
		vDirection = XMVectorSet(-1.f, 0.f, 1.f, 0.f);
		break;
	case Client::DIRECTION::XPZP:
		vDirection = XMVectorSet(1.f, 0.f, 1.f, 0.f);
		break;
	case Client::DIRECTION::XMZM:
		vDirection = XMVectorSet(-1.f, 0.f, -1.f, 0.f);
		break;
	case Client::DIRECTION::XPZM:
		vDirection = XMVectorSet(1.f, 0.f, -1.f, 0.f);
		break;
	case Client::DIRECTION::LAST:
	default:
		break;
	}
	CTransform::Go_Direction(vDirection, fTimeDelta);
}

void CMyTransform::Look_At(DIRECTION eDirection)
{
}

CMyTransform* CMyTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMyTransform* pInstance = new CMyTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMyTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMyTransform::Clone(void* pArg)
{
	CMyTransform* pInstance = new CMyTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMyTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMyTransform::Free()
{
	__super::Free();
}

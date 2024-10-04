#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	PLAYER_DESC* pDesc = static_cast<PLAYER_DESC*>(pArg);


	return S_OK;
}

void CPlayer::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_pTransformCom->Go_Direction(XMVectorSet(1.f, 0.f, 0.f, 0.f), fTimeDelta);
	if (GetKeyState(VK_LEFT) & 0x8000)
		m_pTransformCom->Go_Direction(XMVectorSet(-1.f, 0.f, 0.f, 0.f), fTimeDelta);
	if (GetKeyState(VK_UP) & 0x8000)
		m_pTransformCom->Go_Direction(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta);
	if (GetKeyState(VK_DOWN) & 0x8000)
		m_pTransformCom->Go_Direction(XMVectorSet(0.f, 0.f, -1.f, 0.f), fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}


CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
}

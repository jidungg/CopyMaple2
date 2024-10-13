#include "stdafx.h"
#include "Player.h"
#include "Client_Utility.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CPawn(Prototype)
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

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

void CPlayer::Receive_KeyInput(KEY eKey, KEY_STATE eKeyState, _float fTimeDelta)
{
	XMVECTOR vDir = XMVectorSet(0, 0, 0, 0);

	if (eKey == KEY::RIGHT && eKeyState == KEY_STATE::PRESSING)
		vDir += Get_Direction_Vector(DIR_E);
	if (eKey == KEY::UP && eKeyState == KEY_STATE::PRESSING)
		vDir += Get_Direction_Vector(DIR_N);
	if (eKey == KEY::DOWN && eKeyState == KEY_STATE::PRESSING)
		vDir += Get_Direction_Vector(DIR_S);
	if (eKey == KEY::LEFT && eKeyState == KEY_STATE::PRESSING)
		vDir += Get_Direction_Vector(DIR_W);
	m_pTransformCom->Go_Direction(vDir, fTimeDelta);
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

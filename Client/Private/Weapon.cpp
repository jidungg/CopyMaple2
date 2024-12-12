#include "stdafx.h"
#include "Weapon.h"

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBoneModelObject(pDevice, pContext)
{
}

CWeapon::CWeapon(const CWeapon& Prototype)
	: CBoneModelObject(Prototype)
	, m_pBackSocketMatrix{ Prototype.m_pBackSocketMatrix }

{
}

HRESULT CWeapon::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);
	m_pBackSocketMatrix = pDesc->pBackSocketMatrix;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
  return S_OK;
}

void CWeapon::Compute_Matrix()
{
	assert(nullptr != m_pParentMatrix);
	assert(nullptr != m_pSocketMatrix);
	_matrix		SocketMatrix;
	if(m_bBattle)
		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
	else
		SocketMatrix = XMLoadFloat4x4(m_pBackSocketMatrix);


	for (size_t i = 0; i < 3; i++)
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

}

void CWeapon::Set_Battle(bool bBattle)
{
	m_bBattle = bBattle;
}

CWeapon* CWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon* pInstance = new CWeapon(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBoneModelObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon::Clone(void* pArg)
{
	CWeapon* pInstance = new CWeapon(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBoneModelObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}



#include "Pawn.h"

CPawn::CPawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CPawn::CPawn(const CPawn& Prototype)
	: CGameObject(Prototype)
{
}

void CPawn::Set_Position(const _vector& vPos)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}


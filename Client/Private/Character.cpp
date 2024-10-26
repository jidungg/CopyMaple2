#include"stdafx.h"
#include "Character.h"

CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& Prototype)
	: CPawn(Prototype)
	, m_pAnimStateMachine{ Prototype.m_pAnimStateMachine }
	, m_vMoveDir{ Prototype.m_vMoveDir }
	, m_fJumpPower{ Prototype.m_fJumpPower }
	, m_fBattleTimeMax{ Prototype.m_fBattleTimeMax }
	, m_fRunSpeed{ Prototype.m_fRunSpeed }
	, m_fWalkSpeed{ Prototype.m_fWalkSpeed } 
{
}


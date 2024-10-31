#pragma once
#include "Pawn.h"
BEGIN(Engine)
class CCamera_Trace;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CStateMachine;
class CHumanModelObject;
END
BEGIN(Client)
class CSkill;
class CCharacter :
    public CPawn
{

protected:
	explicit CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;

public:
	virtual void Use_Skill(CSkill* pSkill) abstract;
protected:
	class CModelObject* m_pBody = { nullptr };
	CStateMachine* m_pAnimStateMachine = { nullptr };
	//Status
	XMVECTOR m_vMoveDir{ XMVectorSet(0, 0, 0, 0) };
	_float m_fJumpPower{ 4.5f };
	_float m_fBattleTimeMax{ 4.f };
	_float m_fRunSpeed{ 2.5f };
	_float m_fWalkSpeed{ 1.5f };
	_float m_fFloorHeight{ 1.f };
	_float m_fDetectionRange{ 10.f };

public:
	virtual void Free() override;
};

END
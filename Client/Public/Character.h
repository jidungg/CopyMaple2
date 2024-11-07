#pragma once
#include "Pawn.h"
#include "Status.h"

BEGIN(Engine)
class CCamera_Trace;
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CStateMachine;
class CHumanModelObject;
class CCollider_Sphere;
class CCollider;
class CCollider_Sphere;
END
BEGIN(Client)
class CSkill;
class CCharacter :
    public CPawn
{
public:
	typedef struct CharacterDesc : public CGameObject::GameObjectDesc
	{
		_float fBodyCollisionRadius = { 0.25f };
		_float3 fBodyCollisionOffset = { 0,0.4f ,0};
		_uint iColliderCount = { 1 };
		_uint iBodyColliderIndex = { 0 };
	}CHARACTER_DESC;
protected:
	explicit CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCharacter(const CCharacter& Prototype);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Use_Skill(CSkill* pSkill) abstract;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual _bool Check_Collision(CGameObject* pOther) override;

	_vector Get_MoveDirection() { return m_vMoveDirectionXZ; }
	_vector Get_Position() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	_float Get_BodyCollisionRadius();
	_float3 Get_BodyCollisionOffset();
	_float Get_MoveDistance() { return m_fMoveDistanceXZ; }
	CCollider_Sphere* Get_Collider(_uint iColliderIndex);
protected:
	class CModelObject* m_pBody = { nullptr };
	CStateMachine* m_pAnimStateMachine = { nullptr };

	//실 객체 클래스에서 직접 업데이트 해주어야 하는 속성.
	_vector m_vMoveDirectionXZ{ XMVectorSet(0, 0, 0, 0) };
	_vector m_vLookDirectionXZ{ XMVectorSet(0,0,0,0) };
	//중력은 자동 적용됨. 점프, 에어본 등 필요 시에만 수정해주기.
	_float m_fUpForce{ 0.f };
	
	//자동으로 업데이트 됨.
	_float m_fMoveDistanceXZ{ 0.f };
	_vector m_vNextPos{ 0.f, 0.f, 0.f };
	_float m_fFloorHeight{ 1.f };
	_float m_fCelingHeight{ FLT_MAX };
	_float m_fBoreTimeAcc = 0.f; 

	//Status
	_float m_fBattleTimeMax{ 4.f };
	_float m_fBoreTime{ 10.f };
	STATUS m_tStat;

	//공통 상태 컨트롤 변수
	_bool m_bMove{ false };
	_bool m_bRotate{ true };
	_bool m_bOnFloor{ true };

	vector<CCollider*> m_vecCollider;
private:
	_uint m_iBodyColliderIndex = { 0 };
public:
	virtual void Free() override;
};

END
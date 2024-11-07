#pragma once
#include "StateMachine.h"
BEGIN(Client)
class CMonsterAnimStateMachine :
    public CStateMachine
{
public:
	typedef struct MonsterAnimStateMachineDesc
	{
		unordered_map<MON_STATE, vector<_uint>>* mapAnimIdx ;
		_bool* bDetected = { nullptr };
		_bool* bWalk = { nullptr };
		_bool* bAttack = { nullptr };
		_bool* bMove={ nullptr };
		_bool* bStun = { nullptr };
		_int* iHp = { nullptr };
	}MONSTER_ANIM_STATEMACHINE_DESC;

	static constexpr _tchar m_szProtoTag[] = L"Prototype_MonsterAnimStateMachine";
	enum MON_ANIM_CONDITION
	{
		AC_ANIMENDTRIGGER
		, AC_DETECTED
		, AC_ATTACKTRIGGER
		, AC_ISATTACK
		, AC_MOVE
		, AC_WALK
		, AC_RANDOM
		, AC_BORETRIGGER
		, AC_DAMAGETRIGGER
		, AC_HP
		, AC_STUN
		, AC_LAST
	};

public:
	explicit CMonsterAnimStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CMonsterAnimStateMachine(const CMonsterAnimStateMachine& Prototype);
	virtual ~CMonsterAnimStateMachine() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;
private:
	unordered_map<MON_STATE, vector<_uint>>* m_mapAnimIdx;
	_int m_iRandomCondition = 0;
public:
	static CMonsterAnimStateMachine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
};

END 
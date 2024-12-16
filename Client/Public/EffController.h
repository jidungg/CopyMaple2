#pragma once
#include "Base.h"
#include "Engine_Struct.h"

BEGIN(Client)

class IEffControllable;
class CEffController :
    public CBase
{
	enum PROGRESS_TYPE
	{
		LOOP = 0b00,
		REVERSE = 0b01,
		CLAMP = 0b10,
		LAST
	};
protected:
	explicit CEffController();
	explicit CEffController(const CEffController& Prototype);
	virtual ~CEffController() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream& inFile, const class CEffModel* pModel);
	_bool Update_Controller(_float fTimeDelta) ;

	void Reset_CurrentTrackPosition();
	void Register_AnimEvent(ANIM_EVENT tAnimEvent);

	EFF_CTRL_TYPE Get_CtrlType() { return m_eCtrlType; }
	_float Get_Duration() { return (m_fStopPosition - m_fStartPosition - m_fPhase)/m_fSpeed; }
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }
protected:
	virtual _bool Update_InTime(_float fTrackPos) abstract;

	_bool Is_InTime(_float fTrackPos);


protected:
	EFF_CTRL_TYPE			m_eCtrlType = { EFF_CTRL_TYPE::CT_LAST };
	_float					m_fStartPosition = {0};
	_float					m_fStopPosition = {0};
	_float					m_fSpeed = {1};
	_float					m_fPhase = {0};

	_float					m_fCurrentTrackPos = { 0 };
	IEffControllable* m_pTarget = { nullptr };
	_int						m_iTargetIndex = { -1 };

	PROGRESS_TYPE m_eProgressType = { PROGRESS_TYPE::LOOP };
public:
	virtual CEffController* Clone() abstract;
	virtual void Free() override;
};

END
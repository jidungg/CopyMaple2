#pragma once
#include "Base.h"
#include "Engine_Struct.h"

BEGIN(Client)
enum EFF_TEX_OPERATION_TYPE
{
	TO_TRANSLATE_U,
	TO_TRANSLATE_V,
	TO_ROTATE,
	TO_SCALE_U,
	TO_SCALE_V,
	TO_LAST
};
enum CTRL_TYPE
{
	CT_TRANSFORM,
	CT_ALPHA,
	CT_MATERIAL_COLOR,
	CT_TEXTURE_TRANSFORM,
	CT_LAST
};
class IEffControllable;
class CEffController :
    public CBase
{
protected:
	explicit CEffController();
	explicit CEffController(const CEffController& Prototype);
	virtual ~CEffController() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream& inFile, const class CEffModel* pModel);
	virtual _bool Update_Controller(vector< IEffControllable*>* vecControllable , _float fTimeDelta);

	void Reset_CurrentTrackPosition();
	void Register_AnimEvent(ANIM_EVENT tAnimEvent);

	CTRL_TYPE Get_CtrlType() { return m_eCtrlType; }
protected:
	virtual void Update_InTime(_float fTrackPos) abstract;

	_bool Is_InTime(_float fTrackPos);


protected:
	CTRL_TYPE			m_eCtrlType = { CTRL_TYPE::CT_LAST };
	_float					m_fStartPosition = {0};
	_float					m_fStopPosition = {0};
	_float					m_fSpeed = {1};
	_float					m_fPhase = {0};

	_float					m_fCurrentTrackPos = { 0 };
public:
	virtual CEffController* Clone() abstract;
	virtual void Free() override;
};

END
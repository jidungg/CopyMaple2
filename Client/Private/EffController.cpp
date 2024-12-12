#include "stdafx.h"
#include "EffController.h"

CEffController::CEffController()
{
}

CEffController::CEffController(const CEffController& Prototype)
	: m_eCtrlType(Prototype.m_eCtrlType)
	, m_fStartPosition(Prototype.m_fStartPosition)
	, m_fStopPosition(Prototype.m_fStopPosition)
	, m_fSpeed(Prototype.m_fSpeed)
	, m_fPhase(Prototype.m_fPhase)
	, m_fCurrentTrackPos(Prototype.m_fCurrentTrackPos)
	, m_iTargetIndex(Prototype.m_iTargetIndex)
{
}

HRESULT CEffController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	inFile.read(reinterpret_cast<char*>(&m_fStartPosition), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fStopPosition), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fSpeed), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fPhase), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_iTargetIndex), sizeof(_int));

    return S_OK;
}

_bool CEffController::Update_Controller(_float fTimeDelta)
{
	bool bEnd = false;
	if(m_fCurrentTrackPos >= m_fStartPosition)
		bEnd = Update_InTime(m_fCurrentTrackPos + m_fPhase);

	m_fCurrentTrackPos += fTimeDelta * m_fSpeed ;
	return bEnd;
}

void CEffController::Reset_CurrentTrackPosition()
{
	m_fCurrentTrackPos = 0.f;
}

void CEffController::Register_AnimEvent(ANIM_EVENT tAnimEvent)
{
}

_bool CEffController::Is_InTime(_float fTrackPos)
{
	return (fTrackPos >= m_fStartPosition && fTrackPos <= m_fStopPosition);
}

void CEffController::Free()
{
	__super::Free();
}

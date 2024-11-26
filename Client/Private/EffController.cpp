#include "stdafx.h"
#include "EffController.h"

CEffController::CEffController()
{
}

CEffController::CEffController(const CEffController& Prototype)
{
}

HRESULT CEffController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	inFile.read(reinterpret_cast<char*>(&m_fStartPosition), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fStopPosition), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fSpeed), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fPhase), sizeof(_float));


    return S_OK;
}

_bool CEffController::Update_Controller(_float fTimeDelta)
{
	if (m_fCurrentTrackPos > m_fStopPosition)
	{
		return true;
	}
	if(Is_InTime(m_fCurrentTrackPos))
	{
		Update_InTime(m_fCurrentTrackPos);
	}
	m_fCurrentTrackPos += fTimeDelta * m_fSpeed + m_fPhase;
	return false;
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

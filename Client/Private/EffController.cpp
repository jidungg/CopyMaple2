#include "stdafx.h"
#include "EffController.h"


#define PROGRESS_MASK 0b000000000000011
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
	, m_eProgressType(Prototype.m_eProgressType)
{
}

HRESULT CEffController::Initialize_Prototype(ifstream& inFile, const CEffModel* pModel)
{
	inFile.read(reinterpret_cast<char*>(&m_fStartPosition), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fStopPosition), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fSpeed), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_fPhase), sizeof(_float));
	inFile.read(reinterpret_cast<char*>(&m_iTargetIndex), sizeof(_int));
	_ushort sFlags = 0;
	inFile.read(reinterpret_cast<char*>(&sFlags), sizeof(_ushort));
	sFlags >>= 1;

	m_eProgressType = PROGRESS_TYPE(sFlags & PROGRESS_MASK);


    return S_OK;
}

_bool CEffController::Update_Controller(_float fTimeDelta)
{
	if (m_fCurrentTrackPos >= m_fStopPosition)
	{
		if (m_eProgressType == PROGRESS_TYPE::LOOP)
		{
			m_fCurrentTrackPos = m_fStartPosition;
		}
		else
		{
			return true;
		}
	}

	if(m_fCurrentTrackPos >= m_fStartPosition)
		Update_InTime(m_fCurrentTrackPos );



	m_fCurrentTrackPos += fTimeDelta * m_fSpeed ;
	return false;
}

void CEffController::Reset_CurrentTrackPosition()
{
	m_fCurrentTrackPos = m_fPhase;
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

#include "stdafx.h"
#include "LevelChangeEvent.h"
#include "GameInstance.h"
#include "Level.h"

CLevelChangeEvent::CLevelChangeEvent(CLevel* pLevel)
	:m_pLevel(pLevel)
{
}

CLevelChangeEvent* CLevelChangeEvent::Create(CLevel* pLevel)
{
	return  new CLevelChangeEvent(pLevel);
}

void CLevelChangeEvent::Exec()
{
	CGameInstance::GetInstance()->Open_Level((_int)m_pLevel->Get_LevelID(), m_pLevel);
}

void CLevelChangeEvent::Free()
{
	__super::Free();
}

#include "stdafx.h"
#include "Client_Utility.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "string"
#include <Windows.h> // 추가된 헤더

DirectX::XMVECTOR Client::Get_Direction_Vector(DIRECTION eDirection)
{
	DirectX::XMVECTOR vDirection;
	switch (eDirection)
	{
	case DIRECTION::XM:
		vDirection = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);
		break;
	case DIRECTION::XP:
		vDirection = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
		break;
	case DIRECTION::ZM:
		vDirection = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
		break;
	case DIRECTION::ZP:
		vDirection = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
		break;
	case DIRECTION::XMZP:
		vDirection = DirectX::XMVectorSet(-1.f, 0.f, 1.f, 0.f);
		break;
	case DIRECTION::XPZP:
		vDirection = DirectX::XMVectorSet(1.f, 0.f, 1.f, 0.f);
		break;
	case DIRECTION::XMZM:
		vDirection = DirectX::XMVectorSet(-1.f, 0.f, -1.f, 0.f);
		break;
	case DIRECTION::XPZM:
		vDirection = DirectX::XMVectorSet(1.f, 0.f, -1.f, 0.f);
		break;
	case DIRECTION::YP:
		vDirection = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
		break;
	case DIRECTION::YM:
		vDirection = DirectX::XMVectorSet(0.f, -1.f, 0.f, 0.f);
		break;
	case DIRECTION::LAST:
	default:
		vDirection = DirectX::XMVectorZero();
		break;
	}
	return vDirection;
}

LEVELID Client::Get_CurrentTrueLevel()
{
	auto pGameInstance = CGameInstance::GetInstance();
	LEVELID eLevelID =(LEVELID)pGameInstance->Get_CurrentLevelID();
	if (eLevelID == LEVELID::LEVEL_LOADING)
	{
		eLevelID =  static_cast<CLevel_Loading*>(pGameInstance->Get_CurrentLevel())->Get_NextLevelID();
	}
	return eLevelID;
}




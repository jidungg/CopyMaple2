#pragma once

#include <process.h>


namespace Client
{
	const unsigned int g_iWinSizeX = { 1280 };
	const unsigned int g_iWinSizeY = { 720 };

	enum LEVELID { LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_HOME, LAST };
	enum class TERRAIN_OBJ_TYPE { BLOCK, FILED, PORTAL, SPAWN, LAST };
	enum class DIRECTION { XM, ZM, XP, ZP, XMZP, XPZP, XMZM, XPZM, YP,YM,LAST };
	enum class EVENT_ID { CREATE_OBJ, DEAD_OBJ, LEVEL_CHANGE, LAST };
#define DIR_WS DIRECTION::XM
#define DIR_EN DIRECTION::XP
#define DIR_WN DIRECTION::ZP
#define DIR_ES DIRECTION::ZM
#define DIR_W DIRECTION::XMZP
#define DIR_E DIRECTION::XPZM
#define DIR_N DIRECTION::XPZP
#define DIR_S DIRECTION::XMZM
#define DIR_D DIRECTION::YM
#define DIR_U DIRECTION::YP
	
	enum class ITEM_TYPE { 
		EQUIP = 0, 
		CONSUMABLE = 1, 
		BUILD = 2, 
		ETC = 3, 
		LAST = 4 };

	enum class ITEM_ID
	{
		//건설
		GRASS_A0 = (int)ITEM_TYPE::BUILD * 100,
		ROCK_A0,

		LAST
	};
	typedef struct UIListItemData
	{

	}UIITEMDATA;

}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;

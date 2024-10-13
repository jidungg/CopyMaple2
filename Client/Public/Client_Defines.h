#pragma once

#include <process.h>


namespace Client
{
	const unsigned int g_iWinSizeX = { 1280 };
	const unsigned int g_iWinSizeY = { 720 };

	enum LEVELID { LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_HOME, LEVEL_END };
	enum class TERRAIN_OBJ_TYPE { BLOCK, FILED, PORTAL, SPAWN, TERRAIN_OBJ_END };
	enum class DIRECTION { XM, ZM, XP, ZP, XMZP, XPZP, XMZM, XPZM, YP,YM,DIR_END };

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
	
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;

#pragma once

#include <process.h>

namespace Client
{
	const unsigned int			g_iWinSizeX = { 1280 };
	const unsigned int			g_iWinSizeY = { 720 };

	enum LEVELID {LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_HOME, LEVEL_END };
	enum class TERRAIN_OBJ_TYPE { BLOCK, FILED,PORTAL, SPAWN, TERRAIN_OBJ_END };
	enum class DIRECTION { XM,XP, ZM, ZP,XMZP ,XPZP,XMZM,XPZM , DIR_END };
#define DIR_LD DIRECTION::XM
#define DIR_RU DIRECTION::XP
#define DIR_LU DIRECTION::ZP
#define DIR_RD DIRECTION::ZM
#define DIR_L DIRECTION::XMZP
#define DIR_R DIRECTION::XPZM
#define DIR_U DIRECTION::XPZP
#define DIR_D DIRECTION::XMZM

}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;

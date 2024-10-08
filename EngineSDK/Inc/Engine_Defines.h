#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Effects11\d3dx11effect.h"
#include "DirectXTK\DDSTextureLoader.h"
#include "DirectXTK\WICTextureLoader.h"

#include <d3dcompiler.h>

namespace Engine
{
	enum class PROTOTYPE { PROTO_GAMEOBJ, PROTO_COMPONENT };

	enum class KEY_STATE
	{
		NONE,// 이전에도 눌리지 않았고, 현재도 눌리지 않은.
		DOWN, // 막 누른 시점.
		PRESSING, // 누르고 있는.
		UP, // 막 뗀 시점.
	};

	enum class MOUSE_KEY { LB, RB, MB, LAST, };
	enum class MOUSE_MOVE { X, Y, Z, LAST, };

	enum class KEY // 자체적으로 지원 할 키 종류
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		NUM1,
		NUM2,
		NUM3,
		NUM4,
		NUM5,
		NUM6,
		NUM7,
		NUM8,
		NUM9,
		NUM0,
		NUMPAD8,
		NUMPAD4,
		NUMPAD5,
		NUMPAD6,
		NUMPAD2,
		Q,
		W,
		E,
		R,
		T,
		Y,
		U,
		H,
		J,
		K,
		M,
		I,
		O,
		P,
		A,
		S,
		D,
		F,
		G,
		Z,
		X,
		C,
		V,
		B,
		ALT,
		CTRL,
		LSHIFT,
		SPACE,
		ENTER,
		ESC,
		TAB,
		LAST, // 끝을 알리는.
	};

	enum class CORNOR_TYPE
	{
		LEFT_TOP,
		TOP,
		RIGHT_TOP,
		LEFT,
		CENTER,
		RIGHT,
		LEFT_BOT,
		BOT,
		RIGHT_BOT,
		LAST
	};
}

#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"


#include "json.hpp"
using json = nlohmann::json;
using namespace Engine;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG


#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct
	{
		HINSTANCE		hInstance;
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iNumLevels;
		unsigned int	iViewportWidth;
		unsigned int	iViewportHeight;

	}ENGINE_DESC;
	// 키 정보를 담을 구조체
	struct ENGINE_DLL tKeyInfo
	{
		KEY_STATE   eState; // 키의 상태값
		bool		bPrevPush;	// 이전 프레임에서 눌렸는지 여부.
	};

	typedef struct
	{
		enum TYPE { TYPE_POINT, TYPE_DIRECTOINAL, TYPE_END };
		TYPE			eType;
		XMFLOAT4		vDirection;
		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	}LIGHT_DESC;

	typedef struct
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	}MATERIAL_DESC;


	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[2];
	}VTXPOSTEX;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX;

	//typedef struct
	//{
	//	XMFLOAT3		vPosition;
	//	XMFLOAT2		vTexcoord0;
	//	XMFLOAT2		vTexcoord1;
	//}VTXPOSTEX;



}


#endif // Engine_Struct_h__

#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct EngineDesc
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

	typedef struct LightDesc
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

	typedef struct MaterialDesc
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	}MATERIAL_DESC;


	typedef struct  ENGINE_DLL VertexPositionTexture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[2];
	}VTXPOSTEX;

	typedef struct ENGINE_DLL VertexPositionNormal
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX;

	typedef struct  ENGINE_DLL VertexMesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int					iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXMESH;

	typedef struct ENGINE_DLL Ray
	{
		Ray(const XMFLOAT4& vOrigin, const XMFLOAT4& vDirection)
			: vOrigin(vOrigin), vDirection(vDirection) {}
		Ray(const POINT& tScreenOrigin, const POINT& tScreenDest,  _uint  iWinX, _uint iWinY,  const XMFLOAT4X4& matView, const XMFLOAT4X4& matProj)
			: vOrigin{ 0,0,0,1 }
			,vDirection { 0,0,0,0 }
		{
			float ndcX = tScreenDest.x/ (_float)iWinX *2 -1;
			float ndcY = -tScreenDest.y / (_float)iWinY * 2 + 1;
			XMVECTOR vNear = XMVectorSet(ndcX, ndcY, 0.f, 1.f);
			XMVECTOR vFar = XMVectorSet(ndcX, ndcY, 1.f, 1.f);

			XMMATRIX vInvView = XMMatrixInverse(nullptr, XMLoadFloat4x4(&matView));
			XMMATRIX vInvViewProj = XMMatrixMultiply(vInvView, XMMatrixInverse(nullptr,XMLoadFloat4x4(&matProj)));

			XMStoreFloat4(&vOrigin, XMVector4Transform( XMLoadFloat4( &vOrigin), vInvView));
			XMVECTOR vNearWorld =  XMVector4Transform(vNear, vInvViewProj);
			XMVECTOR vFarWorld = XMVector4Transform(vFar, vInvViewProj);

			XMStoreFloat4( &vDirection,
				XMVector4Normalize(
					vNearWorld
					- vFarWorld
				)
			);
		}

		XMFLOAT4		vOrigin;
		XMFLOAT4		vDirection;
	}RAY;

}


#endif // Engine_Struct_h__

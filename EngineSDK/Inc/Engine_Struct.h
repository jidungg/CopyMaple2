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
	// Ű ������ ���� ����ü
	struct ENGINE_DLL tKeyInfo
	{
		KEY_STATE   eState; // Ű�� ���°�
		bool		bPrevPush;	// ���� �����ӿ��� ���ȴ��� ����.
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
	typedef struct ENGINE_DLL VertexAnimMesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndices;
		XMFLOAT4		vBlendWeights;
		static const unsigned int					iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXANIMMESH;

	typedef struct ENGINE_DLL Ray
	{
		Ray(const XMFLOAT4& vOrigin, const XMFLOAT4& vDirection, _float fDist = 1)
			: fDist(fDist) 
		{
			this->vOrigin  = XMLoadFloat4(&vOrigin);
			this->vDirection = XMLoadFloat4(&vDirection);
		}
		Ray(const FXMVECTOR& vOrigin, const FXMVECTOR& vDirection, _float fDist = 1)
			: fDist(fDist), vOrigin{ vOrigin }, vDirection{ vDirection }
		{
		}
		Ray(const POINT& tScreenDest,  _uint  iWinX, _uint iWinY,  const XMFLOAT4X4& matView, const XMFLOAT4X4& matProj)
			: vOrigin{ 0,0,0,1 }
			,vDirection { 0,0,0,0 }
			, fDist(-1)
		{
			float ndcX = tScreenDest.x/ (_float)iWinX *2 -1;
			float ndcY = -tScreenDest.y / (_float)iWinY * 2 + 1;
			XMVECTOR vNear = XMVectorSet(ndcX, ndcY, 0.f, 1.f);
			XMVECTOR vFar = XMVectorSet(ndcX, ndcY, 1.f, 1.f);

			XMMATRIX invProjMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4( &matProj));
			XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&matView));
			vNear = XMVector3TransformCoord(vNear, invProjMatrix);
			vFar = XMVector3TransformCoord(vFar, invProjMatrix);
			vNear = XMVector3TransformCoord(vNear, invViewMatrix);
			vFar = XMVector3TransformCoord(vFar, invViewMatrix);

			vOrigin =  vNear;
			vDirection = XMVector3Normalize( vFar-vNear);
		}

		XMVECTOR		vOrigin;
		XMVECTOR		vDirection;
		_float				fDist =1;
	}RAY;

	typedef struct ENGINE_DLL RaycastHit
	{
		XMVECTOR		vPoint;
		XMVECTOR		vNormal;
		_float fDist;
		class CColliderBase* pCollider = nullptr;

	}RAYCASTHIT;

	typedef struct Keyframe
	{
		_float			fTrackPosition = { 0 };
	}KEYFRAME;
	typedef struct TransformKeyframe : public KEYFRAME
	{
 		_float3		vScale = { 1,1,1 };
		_float4		vRotation = { 0,0,0,0 };
		_float3		vPosition = { 0,0,0 };

	}TRANSFORM_KEYFRAME;
	typedef struct FloatKeyframe : public KEYFRAME
	{
		_float			fValue = { 0 };
	}FLOAT_KEYFRAME;

	typedef struct AnimEvent
	{
		_float fTime;
		_bool bIsTriggered = false;
		function<void()> pFunc;
	}ANIM_EVENT;
}


#endif // Engine_Struct_h__
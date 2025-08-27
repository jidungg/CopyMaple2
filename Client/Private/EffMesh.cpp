#include "stdafx.h"
#include "EffMesh.h"
#include "EffModel.h"
#include "EffBone.h"
#include "Shader.h"

#define ALPHA_ENABLE_MASK				0b0000000000000001
#define SRC_BLEND_MASK					0b0000000000011110
#define DST_BLEND_MASK					0b0000000111100000
#define ALPHA_TEST_ENABLE_MASK			0b0000001000000000
#define ALPHA_TEST_MASK					0b0001110000000000

#define BLEND_ONE								0b0000
#define BLEND_ZERO							0b0001
#define BLEND_SRC_COLOR					0b0010
#define BLEND_INV_SRC_COLOR			0b0011
#define BLEND_DST_COLOR					0b0100
#define BLEND_INV_DST_COLOR			0b0101
#define BLEND_SRC_ALPHA					0b0110
#define BLEND_INV_SRC_ALPHA			0b0111
#define BLEND_DST_ALPHA					0b1000
#define BLEND_INV_DST_ALPHA			0b1001

#define TEST_ALWAYS							0b000
#define TEST_LESS								0b001
#define TEST_EQUAL								0b010
#define TEST_LEQUAL							0b011
#define TEST_GREATER							0b100
#define TEST_NOTEQUAL						0b101
#define TEST_GEQUAL							0b110
#define TEST_NEVER								0b111

CEffMesh::CEffMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

HRESULT CEffMesh::Initialize_Prototype(CEffModel* pModel, ifstream& inFile, _fmatrix PreTransformMatrix)
{
	_uint iNumController = 0;

	inFile.read(reinterpret_cast<char*>(&m_iMaterialIndex), sizeof(_int));
	//inFile.read(reinterpret_cast<char*>(&iNumController), sizeof(_uint));
	//m_iMaterialControllerIndex.resize(iNumController);
	//inFile.read(reinterpret_cast<char*>(m_iMaterialControllerIndex.data()), sizeof(_int) * iNumController);

	inFile.read(reinterpret_cast<char*>(&m_iTexturingIndex), sizeof(_int));
	//inFile.read(reinterpret_cast<char*>(&iNumController), sizeof(_uint));
	//m_iTextureTransformControllerIndex.resize(iNumController);
	//inFile.read(reinterpret_cast<char*>(m_iTextureTransformControllerIndex.data()), sizeof(_int) * iNumController);

	_uint iNameLength = 0;
	inFile.read(reinterpret_cast<char*>(&iNameLength), sizeof(_uint));
	inFile.read(m_szName, iNameLength);
	m_szName[iNameLength] = '\0';

	m_iNumVertexBuffers = 1;
	inFile.read(reinterpret_cast<char*>(&m_iNumVertices), sizeof(_uint));
	m_iIndexStride = sizeof(_uint);
	_uint iNumFaces = 0;
	inFile.read(reinterpret_cast<char*>(&iNumFaces), sizeof(_uint));
	m_iNumIndices = iNumFaces * 3;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	Ready_VertexBuffer(inFile, pModel);
	Ready_IndexBuffer(inFile, pModel, iNumFaces);

	_ushort sAlphaFlags = 0;
	inFile.read(reinterpret_cast<char*>(&sAlphaFlags), sizeof(_ushort));

	m_tBlendDesc = Read_BlendDescFromFlags(sAlphaFlags);
	m_tDepthStencilDesc = Read_TestDescFromFlags(sAlphaFlags);

	HRESULT hr = m_pDevice->CreateBlendState(&m_tBlendDesc, &m_pBlendState);
	if (FAILED(hr))
		return E_FAIL;
	hr = m_pDevice->CreateDepthStencilState(&m_tDepthStencilDesc, &m_pDepthStencilState);
	if (FAILED(hr))
		return E_FAIL;
	return S_OK;
}

HRESULT CEffMesh::Initialize(void* pArg)
{
	return S_OK;
}


HRESULT CEffMesh::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, const vector<CEffBone*>& Bones)
{
	ZeroMemory(m_BoneMatrices, sizeof(_float4x4) * 512);

	_uint		iNumBones = { 0 };


	for (auto& iBoneIndex : m_BoneIndices)
	{
		XMStoreFloat4x4(&m_BoneMatrices[iNumBones], XMLoadFloat4x4(&m_BoneOffsetMatrices[iNumBones]) * Bones[iBoneIndex]->Get_CombinedTransformationMatrix());
		++iNumBones;
	}

	return pShader->Bind_Matrices(pConstantName, m_BoneMatrices, 512);

}

void CEffMesh::ReSet_OffsetMarix()
{
	for (auto& i : m_BoneOffsetMatrices)
	{
		XMStoreFloat4x4(&i, XMMatrixIdentity());
	}
}

HRESULT CEffMesh::Set_RenderState()
{
	//m_pContext->OMGetBlendState(&m_pOriginalBlendState, m_arrOriginalBlendFactor, m_pOriginalBlendMask);

	//m_pContext->OMSetDepthStencilState(m_pDepthStencilState, 0x00);
	m_pContext->OMSetBlendState(m_pBlendState, m_arrBlendFactor, m_iSampleMask);
	return S_OK;
}

HRESULT CEffMesh::Unset_RenderState()
{
	//m_pContext->OMSetBlendState(m_pOriginalBlendState, m_arrOriginalBlendFactor, *m_pOriginalBlendMask);

	return S_OK;
}

HRESULT CEffMesh::Ready_VertexBuffer(ifstream& inFile, CEffModel* pModel)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		inFile.read(reinterpret_cast<char*>(&pVertices[i].vPosition), sizeof(_float3));
		inFile.read(reinterpret_cast<char*>(&pVertices[i].vNormal), sizeof(_float3));
		inFile.read(reinterpret_cast<char*>(&pVertices[i].vTexcoord), sizeof(_float2));
		inFile.read(reinterpret_cast<char*>(&pVertices[i].vTangent), sizeof(_float3));
		//cout << pVertices[i].vPosition.x<< " " << pVertices[i].vPosition.y<< " " << pVertices[i].vPosition.z << endl;
		//cout << pVertices[i].vNormal.x<< " " << pVertices[i].vNormal.y<< " " << pVertices[i].vNormal.z << endl;
		//cout << pVertices[i].vTexcoord.x<< " " << pVertices[i].vTexcoord.y<< endl;
		//cout << pVertices[i].vTangent.x<< " " << pVertices[i].vTangent.y<< " " << pVertices[i].vTangent.z << endl;

	}
	_uint iNumBones = 0;
	inFile.read(reinterpret_cast<char*>(&iNumBones), sizeof(_uint));

	for (_uint curMeshBoneIdx = 0; curMeshBoneIdx < iNumBones; curMeshBoneIdx++)
	{
		//_uint iNameLength = 0;
		//inFile.read(reinterpret_cast<char*>(&iNameLength), sizeof(_uint));
		//char szName[MAX_PATH] = "";
		//inFile.read(szName, iNameLength);
		//szName[iNameLength] = '\0';
		//cout << szName << endl;
		_uint iBoneIndex = 0;
		inFile.read(reinterpret_cast<char*>(&iBoneIndex), sizeof(_uint));
		m_BoneIndices.push_back(iBoneIndex);

		_float4x4	OffsetMatrix = {};
		inFile.read(reinterpret_cast<char*>(&OffsetMatrix), sizeof(_float4x4));
		//cout << OffsetMatrix._11 << " " << OffsetMatrix._12 << " " << OffsetMatrix._13 << " " << OffsetMatrix._14 << endl;
		//cout << OffsetMatrix._21 << " " << OffsetMatrix._22 << " " << OffsetMatrix._23 << " " << OffsetMatrix._24 << endl;
		//cout << OffsetMatrix._31 << " " << OffsetMatrix._32 << " " << OffsetMatrix._33 << " " << OffsetMatrix._34 << endl;
		//cout << OffsetMatrix._41 << " " << OffsetMatrix._42 << " " << OffsetMatrix._43 << " " << OffsetMatrix._44 << endl;
		//XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		m_BoneOffsetMatrices.push_back(OffsetMatrix);

		//정점마다 어떤 본에 얼마만큼 영향받는지 정보를 읽어온다.
		_uint iNumVertices;
		inFile.read(reinterpret_cast<char*>(&iNumVertices), sizeof(_uint));
		for (_uint curBoneVertex = 0; curBoneVertex < iNumVertices; curBoneVertex++)
		{
			//
			_uint iVertexIdx = 0;
			inFile.read(reinterpret_cast<char*>(&iVertexIdx), sizeof(_uint));
			_float fWeight = 0.f;
			inFile.read(reinterpret_cast<char*>(&fWeight), sizeof(_float));
			//하나의 정점이 영향받을 수 있는 본의 갯수가 최대 4개라고 정함.
			//아무 정보도 들어가지 않으면 0이므로, x부터 0임을 확인하고 순서대로 채워넣는다.
			if (0 == pVertices[iVertexIdx].vBlendWeights.x)
			{
				pVertices[iVertexIdx].vBlendIndices.x = curMeshBoneIdx;
				pVertices[iVertexIdx].vBlendWeights.x = fWeight;
			}
			else if (0 == pVertices[iVertexIdx].vBlendWeights.y)
			{
				pVertices[iVertexIdx].vBlendIndices.y = curMeshBoneIdx;
				pVertices[iVertexIdx].vBlendWeights.y = fWeight;
			}
			else if (0 == pVertices[iVertexIdx].vBlendWeights.z)
			{
				pVertices[iVertexIdx].vBlendIndices.z = curMeshBoneIdx;
				pVertices[iVertexIdx].vBlendWeights.z = fWeight;
			}
			else if (0 == pVertices[iVertexIdx].vBlendWeights.z)
			{
				pVertices[iVertexIdx].vBlendIndices.w = curMeshBoneIdx;
				pVertices[iVertexIdx].vBlendWeights.w = fWeight;
			}
		}
	}

	if (0 == iNumBones)
	{
		iNumBones = 1;

		m_BoneIndices.push_back(pModel->Get_BoneIndex(m_szName));

		_float4x4		OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
		       
		m_BoneOffsetMatrices.push_back(OffsetMatrix);

	}
	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CEffMesh::Ready_IndexBuffer(ifstream& inFile, CEffModel* pModel, _uint iNumFaces)
{
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };


	for (size_t i = 0; i < iNumFaces; i++)
	{
		inFile.read(reinterpret_cast<char*>(&pIndices[iNumIndices++]), sizeof(_uint));
		inFile.read(reinterpret_cast<char*>(&pIndices[iNumIndices++]), sizeof(_uint));
		inFile.read(reinterpret_cast<char*>(&pIndices[iNumIndices++]), sizeof(_uint));
	}

	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

D3D11_BLEND_DESC CEffMesh::Read_BlendDescFromFlags(_ushort& Flags)
{
	D3D11_BLEND_DESC tBlendDesc = {};

	tBlendDesc.AlphaToCoverageEnable = FALSE;
	tBlendDesc.IndependentBlendEnable = TRUE;
	tBlendDesc.RenderTarget[0].BlendEnable = Flags & ALPHA_ENABLE_MASK;
	Flags >>= 1;
	tBlendDesc.RenderTarget[0].SrcBlend =  Read_BlendModeFromFlags(Flags);
	tBlendDesc.RenderTarget[0].DestBlend =Read_BlendModeFromFlags(Flags);
	tBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	tBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


	return tBlendDesc;
}

D3D11_BLEND CEffMesh::Read_BlendModeFromFlags(_ushort& Flag)
{
	_ushort tmpFlag = Flag & 0b00001111;
	D3D11_BLEND tBlend = D3D11_BLEND_ONE;
	switch (tmpFlag)
	{
	case BLEND_ONE:
		tBlend= D3D11_BLEND_ONE;
		break;
	case BLEND_ZERO:
		tBlend = D3D11_BLEND_ZERO;
		break;
	case BLEND_SRC_COLOR:
		tBlend = D3D11_BLEND_SRC_COLOR;
		break;
	case BLEND_INV_SRC_COLOR:
		tBlend = D3D11_BLEND_INV_SRC_COLOR;
		break;
	case BLEND_DST_COLOR:
		tBlend = D3D11_BLEND_DEST_COLOR;
		break;
	case BLEND_INV_DST_COLOR:
		tBlend = D3D11_BLEND_INV_DEST_COLOR;
		break;
	case BLEND_SRC_ALPHA:
		tBlend = D3D11_BLEND_SRC_ALPHA;
		break;
	case BLEND_INV_SRC_ALPHA:
		tBlend = D3D11_BLEND_INV_SRC_ALPHA;
		break;
	case BLEND_DST_ALPHA:
		tBlend = D3D11_BLEND_DEST_ALPHA;
		break;
	case BLEND_INV_DST_ALPHA:
		tBlend = D3D11_BLEND_INV_DEST_ALPHA;
		break;
	default:
		tBlend = D3D11_BLEND_ONE;
	}
	Flag >>= 4;
	return tBlend;
}

D3D11_DEPTH_STENCIL_DESC CEffMesh::Read_TestDescFromFlags(_ushort& Flag)
{
	D3D11_DEPTH_STENCIL_DESC tTestDesc = {};

	tTestDesc.DepthEnable = (Flag & 0b1);
	Flag >>= 1;
	tTestDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tTestDesc.DepthFunc = Read_TestModeFromFlags(Flag);

	tTestDesc.DepthEnable = true;
	//tTestDesc.DepthFunc = D3D11_COMPARISON_LESS;
	return tTestDesc;
}

D3D11_COMPARISON_FUNC CEffMesh::Read_TestModeFromFlags(_ushort& Flag)
{
	_ushort tmpFlag = Flag & 0b111;
	D3D11_COMPARISON_FUNC eTestFunc = D3D11_COMPARISON_LESS;

	switch (tmpFlag)
	{
	case TEST_ALWAYS:
		eTestFunc = D3D11_COMPARISON_ALWAYS;
		break;
	case TEST_LESS:
		eTestFunc = D3D11_COMPARISON_GREATER;
		break;
	case TEST_EQUAL:
		eTestFunc = D3D11_COMPARISON_EQUAL;
		break;
	case TEST_LEQUAL:
		eTestFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case TEST_GREATER:
		eTestFunc = D3D11_COMPARISON_LESS;
		break;
	case TEST_NOTEQUAL:
		eTestFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case TEST_GEQUAL:
		eTestFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case TEST_NEVER:
		eTestFunc = D3D11_COMPARISON_NEVER;
		break;
	default:
		break;
	}
	Flag >>= 3;
	return eTestFunc;
}

CEffMesh* CEffMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CEffModel* pModel, ifstream& inFile, _fmatrix PreTransformMatrix)
{

	CEffMesh* pInstance = new CEffMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype( pModel, inFile, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CEffMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CEffMesh::Clone(void* pArg)
{
	return nullptr;
}

void CEffMesh::Free()
{
	__super::Free();
	Safe_Release(m_pBlendState);
	Safe_Release(m_pDepthStencilState);
}

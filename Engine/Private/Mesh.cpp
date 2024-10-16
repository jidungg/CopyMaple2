#include "..\Public\Mesh.h"
#include "Model.h"
#include "GameInstance.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, CModel* pModel, ifstream& inFile )
{
	inFile.read(reinterpret_cast<char*>(&m_iMaterialIndex), sizeof(_uint));

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

#pragma region VERTEX_BUFFER	

	HRESULT hr = CModel::TYPE_NONANIM == eModelType ? Ready_VertexBuffer_For_NonAnim(inFile) : Ready_VertexBuffer_For_Anim(inFile, pModel);

	if (FAILED(hr))
		return E_FAIL;


#pragma endregion

#pragma region INDEX_BUFFER
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


	for (size_t i = 0; i <iNumFaces; i++)
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

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_NonAnim(ifstream& inFile)
{

	m_iVertexStride = sizeof(VTXMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;



	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);
	XMMATRIX PreTransformMatrix = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_GLOBAL);
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		inFile.read(reinterpret_cast<char*>(&pVertices[i].vPosition), sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		inFile.read(reinterpret_cast<char*>(&pVertices[i].vNormal), sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		inFile.read(reinterpret_cast<char*>(&pVertices[i].vTexcoord), sizeof(_float2));
		inFile.read(reinterpret_cast<char*>(&pVertices[i].vTangent), sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PreTransformMatrix));

	}

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_For_Anim(ifstream& inFile, CModel* pModel)
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
	}
	inFile.read(reinterpret_cast<char*>(&m_iNumBones), sizeof(_uint));

	for (size_t curMeshBoneIdx = 0; curMeshBoneIdx < m_iNumBones; curMeshBoneIdx++)
	{
		_uint iNameLength = 0;
		inFile.read(reinterpret_cast<char*>(&iNameLength), sizeof(_uint));
		char szName[MAX_PATH] = "";
		inFile.read(szName, iNameLength);
		szName[iNameLength] = '\0';

		m_BoneIndices.push_back(pModel->Get_BoneIndex(szName));

		_uint iNumVertices;
		inFile.read(reinterpret_cast<char*>(&iNumVertices), sizeof(_uint));
		for (size_t curBoneVertex = 0; curBoneVertex < iNumVertices; curBoneVertex++)
		{
			//
			_uint iVertexIdx = 0;
			inFile.read(reinterpret_cast<char*>(&iVertexIdx), sizeof(_uint));
			_float fWeight = 0.f;
			inFile.read(reinterpret_cast<char*>(&fWeight), sizeof(_float));
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

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}



CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, CModel* pModel, ifstream& inFile)
{

	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModel, inFile)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	return nullptr;
}

void CMesh::Free()
{
	__super::Free();
}

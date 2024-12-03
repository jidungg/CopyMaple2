#include "stdafx.h"
#include "EffBone.h"
#include "EffController.h"
#include "GameInstance.h"

CEffBone::CEffBone()
{
	m_pGameInstance = CGameInstance::GetInstance();
}

CEffBone::CEffBone(const CEffBone& Prototype)
	:m_TransformationMatrix(Prototype.m_TransformationMatrix)
	, m_DefaultTransformationMatrix(Prototype.m_DefaultTransformationMatrix)
	, m_iParentBoneIndex(Prototype.m_iParentBoneIndex)
	, m_bBillboard(Prototype.m_bBillboard)
	, m_pGameInstance(Prototype.m_pGameInstance)
{
	strcpy_s(m_szName, Prototype.m_szName);
	XMStoreFloat4x4(&m_CombindTransformationMatrix, XMMatrixIdentity());
}

HRESULT CEffBone::Initialize(ifstream& inFile, _int iParentBoneIndex)
{
	_uint iNameLength = 0;
	inFile.read((char*)&iNameLength, sizeof(_uint));
	//cout << iNameLength << endl;
	inFile.read((char*)&m_szName, iNameLength);
	m_szName[iNameLength] = '\0';
	//cout << " Bone Name : "<< m_szName << endl;

	//_uint iNumControls = 0;
	//inFile.read((char*)&iNumControls, sizeof(_uint));
	//m_vecTrasnformControlIndex.resize(iNumControls);
	//inFile.read((char*)m_vecTrasnformControlIndex.data(), sizeof(_uint) * iNumControls);

	inFile.read((char*)&m_bBillboard, 1);
	inFile.read(reinterpret_cast<char*>(&m_DefaultTransformationMatrix), sizeof(_float4x4));
	m_TransformationMatrix = m_DefaultTransformationMatrix;
	if (m_bBillboard)
	{
		cout << "Billboard" << endl;
		//cout << m_TransformationMatrix._11 << ", "<<m_TransformationMatrix._12 << ", " << m_TransformationMatrix._13 << ", " << m_TransformationMatrix._14 << endl;
		//cout << m_TransformationMatrix._21 << ", " << m_TransformationMatrix._22 << ", " << m_TransformationMatrix._23 << ", " << m_TransformationMatrix._24 << endl;
		//cout << m_TransformationMatrix._31 << ", " << m_TransformationMatrix._32 << ", " << m_TransformationMatrix._33 << ", " << m_TransformationMatrix._34 << endl;
		//cout << m_TransformationMatrix._41 << ", " << m_TransformationMatrix._42 << ", " << m_TransformationMatrix._43 << ", " << m_TransformationMatrix._44 << endl;

	}

	//XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	XMStoreFloat4x4(&m_CombindTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CEffBone::Update_CombinedTransformationMatrix(const vector<CEffBone*>& Bones, _fmatrix PreTransformMatrix)
{
	//루트 본 인 경우 -> PreTransformMatrix 만 곱하기
	if (-1 == m_iParentBoneIndex)
	{
		XMStoreFloat4x4(&m_CombindTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
	}
	//루트가 아닌 경우 -> 부모 본의 CombindTransformationMatrix 와 현재 본의 TransformationMatrix 를 곱하기
	else
	{
		XMStoreFloat4x4(&m_CombindTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) *
		XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombindTransformationMatrix));
	}
	if (m_bBillboard)
	{

		//_vector vLook = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW).r[2];
		//vLook = -vLook;
		_vector vLook{ -1.f, 0, -1.f ,0};
		vLook = XMVector4Normalize(vLook);
		_vector vUp = { 0.f, 1.f, 0.f, 0.f };
		_vector vRight = DirectX::XMVector4Normalize(DirectX::XMVector3Cross(vUp, vLook));
		vRight.m128_f32[3] = 0;
		vUp = DirectX::XMVector4Normalize(DirectX::XMVector3Cross(vLook, vRight));
		vUp.m128_f32[3] = 0;

		_float3 vScale =  _float3(DirectX::XMVectorGetX(DirectX::XMVector3Length(XMLoadFloat4x4(&m_CombindTransformationMatrix).r[0])),
			DirectX::XMVectorGetX(DirectX::XMVector3Length(XMLoadFloat4x4(&m_CombindTransformationMatrix).r[1])),
			DirectX::XMVectorGetX(DirectX::XMVector3Length(XMLoadFloat4x4(&m_CombindTransformationMatrix).r[2])));
		vRight = vRight * vScale.x;
		vUp = vUp * vScale.y;
		vLook = vLook * vScale.z;

		memcpy( m_CombindTransformationMatrix.m[0] ,&vRight, sizeof(_float4));
		memcpy( m_CombindTransformationMatrix.m[1] ,&vUp, sizeof(_float4));
		memcpy( m_CombindTransformationMatrix.m[2] ,&vLook, sizeof(_float4));
		
	}
}



CEffBone* CEffBone::Create(ifstream& inFile, _int iParentBoneIndex)
{
	CEffBone* pInstance = new CEffBone();

	if (FAILED(pInstance->Initialize(inFile, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CEffBone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CEffBone* CEffBone::Clone()
{
	return new CEffBone(*this);
}

void CEffBone::Free()
{
	__super::Free();
}

void CEffBone::Reset()
{
	m_TransformationMatrix = m_DefaultTransformationMatrix;
}

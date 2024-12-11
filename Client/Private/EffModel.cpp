#include "stdafx.h"
#include "EffModel.h"
#include "EffBone.h"
#include "EffMesh.h"
#include "EffMaterialProperty.h"
#include "EffTexturingProperty.h"
#include "EffConcreteController.h"
#include "EffModel.h"
#include "Shader.h"

CEffModel::CEffModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}
//원본에만 있어도 되는 데이터 : 메쉬, 텍스처
//객체마다 필요한 데이터 : 매터ㅑ리얼 Prop, TexturingProp,  컨트롤 , 본, Alpha, MaterialColor, TextureTransform
//TexturingProperty의 Texture는 얕은 복사, 나머ㅏ지는 깊은 복사
CEffModel::CEffModel(const CEffModel& Prototype)
    : CComponent(Prototype)
    , m_PreTransformMatrix(Prototype.m_PreTransformMatrix)
    , m_vecMesh{ Prototype.m_vecMesh }
	, m_iNumMeshes(Prototype.m_iNumMeshes)
{
    for (auto& pBone : Prototype.m_vecBone)
        m_vecBone.push_back(pBone->Clone());

    for (auto& pMesh : m_vecMesh)
        Safe_AddRef(pMesh);

    for (auto& pMaterial : Prototype.m_vecMaterial)
        m_vecMaterial.push_back(static_cast<CEffMaterialProperty*>(pMaterial->Clone(nullptr)));

    for (auto& pTexturing : Prototype.m_vecTexturing)
        m_vecTexturing.push_back(static_cast<CEffTexturingProperty*>(pTexturing->Clone(nullptr)));

    for (auto& pControl : Prototype.m_vecControl)
        m_vecControl.push_back(pControl->Clone());
}

HRESULT CEffModel::Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);
    //XMStoreFloat4x4(&m_PreTransformMatrix, XMMatrixScaling(1 , 1 , 1 ) * XMLoadFloat4x4(&m_PreTransformMatrix));
    std::ifstream inFile(pModelFilePath, std::ios::binary);
    if (!inFile) {
        string str = "파일을 열 수 없습니다.";
        str += pModelFilePath;
        MessageBoxA(NULL, str.c_str(), "에러", MB_OK);
        return E_FAIL;
    }
    bool bAnim;
    inFile.read(reinterpret_cast<char*>(&bAnim), sizeof(bool));

    if (FAILED(Ready_Bones(inFile, -1)))
        return E_FAIL;

    if (FAILED(Ready_Meshes(inFile)))
        return E_FAIL;

    if (FAILED(Ready_Materials(inFile)))
        return E_FAIL;

    if (FAILED(Ready_Texturings(inFile, pModelFilePath)))
        return E_FAIL;

    if (FAILED(Ready_Controls(inFile)))
        return E_FAIL;

    inFile.close();

    return S_OK;
}

HRESULT CEffModel::Initialize(void* pArg)
{
    for (auto& pController : m_vecControl)
    {
        EFF_CTRL_TYPE eType = pController->Get_CtrlType();
        switch (eType)
        {
        case Client::CT_TRANSFORM:
            static_cast<CEffTransformController*>(pController)->Set_Target(m_vecBone);
            break;
        case Client::CT_ALPHA:
            static_cast<CEffAlphaController*>(pController)->Set_Target(m_vecMaterial);
            break;
        case Client::CT_MATERIAL_COLOR:
            static_cast<CEffMaterialColorController*>(pController)->Set_Target(m_vecMaterial);
            break;
        case Client::CT_TEXTURE_TRANSFORM:
            static_cast<CEffTextureTransfromController*>(pController)->Set_Target(m_vecTexturing);
            break;
        case Client::CT_LAST:
            break;
        default:
            break;
        }
        m_fDuration = max(m_fDuration, pController->Get_Duration());
    }
    return S_OK;
}


HRESULT CEffModel::Render(CShader* pShader)
{
    for (_int i = m_iNumMeshes -1; i >= 0 ; i--)
    {
        _uint iMaterialIdx = m_vecMesh[i]->Get_MaterialIndex();
        if(iMaterialIdx != -1)
            if (FAILED(m_vecMaterial[iMaterialIdx]->Bind_Material(pShader)))
                return E_FAIL;
        _uint iTexturingIdx = m_vecMesh[i]->Get_TexturingIndex();
        if(iTexturingIdx != -1)
            if (FAILED(m_vecTexturing[iTexturingIdx]->Bind_Texture(pShader)))
                return E_FAIL;
        if (FAILED(m_vecMesh[i]->Bind_BoneMatrices(pShader, "g_BoneMatrices", m_vecBone)))
            return E_FAIL;

        if (FAILED(pShader->Begin(0)))
            return E_FAIL;
        if (FAILED(m_vecMesh[i]->Bind_BufferDesc()))
            return E_FAIL;
        if (FAILED((m_vecMesh[i]->Render())))
            return E_FAIL;
    }

    return S_OK;
}



void CEffModel::Update_Animation(_float fTimeDelta, _fmatrix matWorld)
{
    if (m_bPlay)
    {
        _bool bAnimEnd = true;
        //모든 컨트롤러가 끝나야 true. -> 하나라도 false면 안됨
        //현재 프레임 계산
        _uint iCOntrolIndex = 0;
        for (auto& pController : m_vecControl)
        {
            bAnimEnd = pController->Update_Controller(fTimeDelta) && bAnimEnd;
            iCOntrolIndex++;
        }


        if (bAnimEnd)
        {
            if (m_bLoop)
                Reset();
            else
            {
                Stop_Animation();
            }
        }

        //뼈들의 합성변환행렬을 갱신
        for (auto& pBone : m_vecBone)
        {
            pBone->Update_CombinedTransformationMatrix(m_vecBone, XMLoadFloat4x4(&m_PreTransformMatrix), matWorld);
        }
        for (auto& tEvnt : m_listAnimEvent)
        {
            if (tEvnt.bIsTriggered)
                continue;
            if ((m_fCurrentTrackPosition / m_fDuration) >= tEvnt.fTime)
            {
                tEvnt.pFunc();
                tEvnt.bIsTriggered = true;
            }
        }
		m_fCurrentTrackPosition += fTimeDelta;
    }
}

void CEffModel::Start_Animation()
{
	m_bPlay = true;
    Set_Active(true);
    Reset();
}

void CEffModel::Stop_Animation()
{
    for (auto& callback : m_listAnimEndCallBack)
        callback(this);
    m_bPlay = false;
    Set_Active(false);
}

void CEffModel::Reset()
{
    for (auto& pBone : m_vecBone)
        pBone->Reset();
    for (auto& pMaterial : m_vecMaterial)
		pMaterial->Reset();
	for (auto& pTexturing : m_vecTexturing)
		pTexturing->Reset();
    for (auto& pController : m_vecControl)
        pController->Reset_CurrentTrackPosition();

}

void CEffModel::Register_AnimEvent(ANIM_EVENT tAnimEvent)
{
    m_listAnimEvent.push_back(tAnimEvent);
}


_uint CEffModel::Get_BoneIndex(const _char* pBoneName) const
{
    _uint	iBoneIndex = { 0 };

    auto	iter = find_if(m_vecBone.begin(), m_vecBone.end(), [&](CEffBone* pBone)->_bool
        {
            if (false == strcmp(pBone->Get_Name(), pBoneName))
                return true;

            ++iBoneIndex;

            return false;
        });

    if (iter == m_vecBone.end())
        MSG_BOX("그런 뼈가 없어");


    return iBoneIndex;
}

void CEffModel::Set_AnimSpeed(_float fSpeed)
{
	for (auto& pController : m_vecControl)
		pController->Set_Speed(fSpeed);
}

HRESULT CEffModel::Ready_Bones(ifstream& inFile, _uint iParentBoneIndex)
{
    CEffBone* pBone;

    pBone = CEffBone::Create(inFile, iParentBoneIndex);

    if (nullptr == pBone)
        return E_FAIL;

    m_vecBone.push_back(pBone);

    iParentBoneIndex = (_uint)m_vecBone.size() - 1;
    _uint iNumChildren = 0;
    inFile.read(reinterpret_cast<char*>(&iNumChildren), sizeof(_uint));
    for (_uint i = 0; i < iNumChildren; ++i)
    {
        Ready_Bones(inFile, iParentBoneIndex);
    }

    return S_OK;
}

HRESULT CEffModel::Ready_Meshes(ifstream& inFile)
{
    inFile.read(reinterpret_cast<char*>(&m_iNumMeshes), sizeof(_uint));
    for (_uint i = 0; i < m_iNumMeshes; i++)
    {
        CEffMesh* pMesh = CEffMesh::Create(m_pDevice, m_pContext, this, inFile, XMLoadFloat4x4(&m_PreTransformMatrix));
        if (nullptr == pMesh)
            return E_FAIL;

        m_vecMesh.push_back(pMesh);
    }

    return S_OK;
}

HRESULT CEffModel::Ready_Materials(ifstream& inFile)
{
	_uint iNumMaterials = 0;
    inFile.read(reinterpret_cast<char*>(&iNumMaterials), sizeof(_uint));

    m_vecMaterial.resize(iNumMaterials);

    for (_uint i = 0; i < iNumMaterials; i++)
    {
        CEffMaterialProperty* pMaterial = CEffMaterialProperty::Create(m_pDevice, m_pContext, inFile);
        m_vecMaterial[i] = pMaterial;
    }

    return S_OK;
}

HRESULT CEffModel::Ready_Texturings(ifstream& inFile, const _char* pModelFilePath)
{
	_uint iNumTexturings = 0;
    inFile.read(reinterpret_cast<char*>(&iNumTexturings), sizeof(_uint));
    m_vecTexturing.resize(iNumTexturings);
    _char		szDrive[MAX_PATH] = "";
    _char		szDirectory[MAX_PATH] = "";
    _splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
    strcat_s(szDrive, szDirectory);
    for (_uint i = 0; i < iNumTexturings; i++)
    {
        CEffTexturingProperty* pTexturing = CEffTexturingProperty::Create(m_pDevice, m_pContext, szDrive, inFile);
        m_vecTexturing[i] = pTexturing;
    }

    return S_OK;
}


HRESULT CEffModel::Ready_Controls(ifstream& inFile)
{
    _uint iNumControls = 0;
    inFile.read(reinterpret_cast<char*>(&iNumControls), sizeof(_uint));
    m_vecControl.resize(iNumControls);
    for (_uint i = 0; i < iNumControls; i++)
    {
		EFF_CTRL_TYPE eCtrlType;
		inFile.read(reinterpret_cast<char*>(&eCtrlType), sizeof(_uint));
        CEffController* pMaterial = nullptr;
        switch (eCtrlType)
        {
        case Client::CT_TRANSFORM:
			pMaterial = CEffTransformController::Create(inFile, this);
            break;
        case Client::CT_ALPHA:
			pMaterial = CEffAlphaController::Create(inFile, this);
            break;
        case Client::CT_MATERIAL_COLOR:
			pMaterial = CEffMaterialColorController::Create(inFile, this);
            break;
        case Client::CT_TEXTURE_TRANSFORM:
			pMaterial = CEffTextureTransfromController::Create(inFile, this);
            break;
        case Client::CT_LAST:
        default:
            break;
        }
       if (nullptr == pMaterial)
           return E_FAIL;
        m_vecControl[i] = pMaterial;
    }
    return S_OK;
}

CEffModel* CEffModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
    CEffModel* pInstance = new CEffModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pModelFilePath, PreTransformMatrix)))
    {
        MSG_BOX("Failed to Created : CEffectModel");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CEffModel::Clone(void* pArg)
{
    CEffModel* pInstance = new CEffModel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CEffectModel");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEffModel::Free()
{
	__super::Free();
    for (auto& pBone : m_vecBone)
        Safe_Release(pBone);
    m_vecBone.clear();
    for (auto& pMesh : m_vecMesh)
        Safe_Release(pMesh);
    m_vecMesh.clear();
    for (auto& mat : m_vecMaterial)
        Safe_Release(mat);
    m_vecMaterial.clear();
	for (auto& tex : m_vecTexturing)
		Safe_Release(tex);
	m_vecTexturing.clear();
	for (auto& control : m_vecControl)
		Safe_Release(control);
	m_vecControl.clear();
}

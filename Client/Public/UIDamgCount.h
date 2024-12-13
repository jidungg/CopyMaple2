#pragma once
#include "WorldUIObject.h"

BEGIN(Engine)
class CRenderTarget;
class CTarget_Manager;
class CTexture;
END
BEGIN(Client)
class CUIDamgCount :
    public CWorldUIObject
{

public:
	typedef struct UIDmgCountDesc : public CWorldUIObject::WorldUIObjDesc
	{
		_int iDamg = { 0 };
		_float fLifeTime = { 1.f };
		DAMG_TYPE eDamgType = DAMG_TYPE::LAST;
	}UIDMGCOUNT_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UIDmgCountDesc";
protected:
	CUIDamgCount(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIDamgCount(const CUIDamgCount& Prototype);
	virtual ~CUIDamgCount() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_Damge(_int iValue);
	void Start();
	_bool Is_Critical() { return m_eDamgType == DAMG_TYPE::PLAYER_CRITICAL || m_eDamgType == DAMG_TYPE::MONSTER_CRITICAL; }
	_bool Is_Player() { return m_eDamgType == DAMG_TYPE::PLAYER_CRITICAL || m_eDamgType == DAMG_TYPE::PLAYER_NORMAL; }
protected:
	virtual HRESULT Bind_ShaderResources(CShader* pShader) override;
	HRESULT Ready_Components(void* pArg);

private:
	DAMG_TYPE m_eDamgType = { DAMG_TYPE::LAST };
	vector<_char> m_vecNumbers;
	_int m_iDigitCount = { 0 };

	_float m_fRisingSpeed = { 10.f };
	_float m_fLifeTime = { 1.f };
	_float m_fLifeTimeAcc = { 0.f };

	_int m_iNumTextureHeight = { 54 };
	_int m_iNumTextureStride= { 54 };

	CTexture* m_pTextureCom = { nullptr };
	_int* m_arrNumTextureWidth;
public:
	static CUIDamgCount* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;

};

END
#pragma once
#include "Client_Defines.h"
#include "Pawn.h"
#include "Item.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CModel;
class CVIBuffer_Rect;
class CCollider;
END

BEGIN(Client)
class CModelObject;
class CItem;
class CCubeTerrain;
class CTerrainObject;
class CBuildPreview;
class CBuilder :
    public CPawn
{
	public:
		enum class ANIM_STATE {
			IDLE,
			LAST
		};
public:
	typedef struct tagBuilderDesc : public CPawn::GAMEOBJECT_DESC
	{
		CCubeTerrain* pCubeTerrain = nullptr;

	}BUILDER_DESC;

	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_Builder";
protected:
	explicit CBuilder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBuilder(const CBuilder& Prototype);
	virtual ~CBuilder() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Receive_KeyInput(_float fTimeDelta)  override;
	virtual void Late_Update(_float fTimeDelta) override;

	void Set_BuildItem(const ITEM_DESC* tItemDesc);
	void Move_To(const _vector& vPos);

private:
	HREFTYPE Ready_Builder();
	HREFTYPE Ready_Preview(const _char* szModelTag);
private:
	CCubeTerrain* m_pCubeTerrain = { nullptr };

	CModelObject* m_pBird = { nullptr };
	XMVECTOR m_vBirdOffset = XMVectorSet(0, 1, 0, 1);

	CBuildPreview* m_pPreview = { nullptr };
	XMVECTOR m_vPreviewOffset = XMVectorSet(0, 0.5f, 0, 1);
	_char m_szItemName[MAX_PATH] = "";
	_char m_szBuildItemTag[MAX_PATH] = "";
	int m_iBuildData = 0;
	XMVECTOR m_vMoveDir = XMVectorSet(0, 0, 0, 0);
public:
	static CBuilder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
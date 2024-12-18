#pragma once
#include "Base.h"
#include "EffModelObject.h"

#include "ObjectPool.h"
BEGIN(Engine)
class CGameInstance;
class CGameObject;
END
BEGIN(Client)
class CUIDamgCount;
class CEffectManager :
    public CBase
{
	DECLARE_SINGLETON(CEffectManager)
private:
	CEffectManager();
	virtual ~CEffectManager() = default;
	  
public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Update(_float fTimeDelta);

	HRESULT Load_Data();

	void Play_EffectModel(EFF_MODEL_ID eID, _vector vPos = {0,0,0,1}, _vector vRotation ={ 0,0,0,0 }, _float fScale = 1);
	void Play_DamgCount(DAMG_TYPE eID, _int iDamg, _vector vPos = { 0,0,0,1 }, _vector vRotation = { 0,0,0,0 }, _float fScale = 1);
	void Play_DamgCount(_bool bCrit, _bool bPlayer, _int iDamg, _vector vPos = { 0,0,0,1 }, _vector vRotation = { 0,0,0,0 }, _float fScale = 1);
	void Play_RecoverCount(_int iRecovery, _vector vPos = { 0,0,0,1 }, _vector vRotation = { 0,0,0,0 }, _float fScale = 1);
	//void Play_EffectModel(EFF_MODEL_ID eID, CGameObject* pTarget = {nullptr}, _vector vPos={ 0,0,0,1 }, _vector vRotation= { 0,0,0,0 }, _float fScale = 1);
private:

	CGameInstance* m_pGameInstance = { nullptr };
	map<EFF_MODEL_ID, CObjectPool<CEffModelObject>*> m_mapEffectModel;
	list<pair<EFF_MODEL_ID,CEffModelObject*>> m_listPlayingEffect;

	CObjectPool<CUIDamgCount>* m_DmgCountPool[(_uint)DAMG_TYPE::LAST];
	list<pair<DAMG_TYPE, CUIDamgCount*>> m_listPlayingDamgCount;

public:
	void Free();
};
#define EFFECT_MANAGER CEffectManager::GetInstance()
END
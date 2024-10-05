#pragma once

#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"

/* 원형객체들을 레벨별로 관리하겠다.*/
/* 특정 레벨에서 복제할 필요가 없는 원형들은 삭제하겠다. */
/* 지정한 객체를 복제하여 리턴한다. */


BEGIN(Engine)

class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(Engine::PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	class CBase* Clone_Stock_Proto_Object(const _wstring& strPrototypeTag, void* pArg);
	class CBase* Clone_Stock_Proto_Component(const _wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);
private:	
	_uint										m_iNumLevels = { 0 };
	map<const _wstring, class CBase*>*			m_pPrototypes = { nullptr };
	typedef map<const _wstring, class CBase*>	PROTOTYPE;

private:
	class CBase* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);



public:
	static CPrototype_Manager* Create(_uint iNumLevels);
	virtual void Free(); 

};

END
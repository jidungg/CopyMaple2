#pragma once
#include "UIObject.h"


BEGIN(Client)
class CUIQuestGuide;
class CUIQuestGuideBundle :
    public CUIObject
{
public:
	typedef struct UIQuestGuideBundleDesc : public CUIObject::UIOBJECT_DESC
	{
	
	}QUESTGUIDEBUNDLE_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIQuestGuideBundle";
protected:
	CUIQuestGuideBundle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIQuestGuideBundle(const CUIQuestGuideBundle& Prototype);
	virtual ~CUIQuestGuideBundle() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	void Add_QuestGuide(QUEST_ID eQuestID);
	void Remove_QuestGuide(QUEST_ID eQuestID);
	void Update_Data();
	void Update_Data(QUEST_ID eQuestID);
private:
	void Arrange_Guide();
private:
	map< QUEST_ID,CUIQuestGuide*> m_mapQuestGuide;

	_float m_fGuideWidth = {296};
public:
	static CUIQuestGuideBundle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END
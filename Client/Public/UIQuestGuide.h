#pragma once
#include "UIPanel.h"
#include "QuestDataBase.h"
BEGIN(Client)
class CUIFont;
class CUIIcon;
class CUIQuestGuide :
    public CUIObject
{
public:
	typedef struct QuestGuideDesc : public CUIObject::UIOBJECT_DESC
	{
		QUEST_ID eQuestID;
	}QUESGUIDE_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_CUIQuestMeter";
protected:
	CUIQuestGuide(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIQuestGuide(const CUIQuestGuide& Prototype);
	virtual ~CUIQuestGuide() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render()override;

	_float Get_Height();
	void Update_Data();

private:
	HRESULT Ready_Components();

protected:
	CQuestDataBase* m_pQuestDB = { nullptr };
	QUEST_DATA* m_pQuestData = { nullptr };

	CUIPanel* m_pBackPanel = { nullptr };
	CUIPanel* m_pTitlePanel = { nullptr };
	CUIPanel* m_pHighlightPanel = { nullptr };
	CUIFont* m_pTitleFont = { nullptr };

	CUIFont* m_pContentFont = { nullptr };

	_float2 m_f2TitleSize = { 200,30 };
	_float m_fCommonPadding = { 5 };
public:
	static CUIQuestGuide* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
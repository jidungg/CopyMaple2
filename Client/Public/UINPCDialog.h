#pragma once
#include "UIContainer.h"
#include "Conversation.h"

BEGIN(Client)
class CUIPanel;
class CUIFont;
class CUIChatOption;
class CUIButton;
class CNPC;
class CUIIcon;
class CUINPCDialog :
    public CUIContainer
{
public:
	typedef struct NPCDialogDesc : public CUIContainer::UIOBJECT_DESC
	{

	}NPCDIALOG_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_UINPCDialog";
protected:
	CUINPCDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUINPCDialog(const CUINPCDialog& Prototype);
	virtual ~CUINPCDialog() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_NPC(CNPC* pNPC) { m_pNPC = pNPC; }
	void Set_ConversationNode(const CONVERSATION_NODE_DATA& tNode);


	void To_NextConversation();
	void On_OptionSelected(void* pArg);
	void Escape();

private:
	_float2 Get_OptionOffset(_uint iOptionIndex, const CONVERSATION_NODE_DATA& tNode);
private:
	CNPC* m_pNPC = { nullptr };

	CUIPanel* m_pLeftPortrait = { nullptr };
	CUIPanel* m_pRightPortrait = { nullptr };

	CUIPanel* m_pChatBox = { nullptr };

	CUIFont* m_pNPCName = { nullptr };
	CUIFont* m_pScript = { nullptr };
	vector< CUIChatOption*> m_vecUIOption;

	CUIIcon* m_pHighlighter = { nullptr };

	CUIButton* m_pESCButton = { nullptr };
	CUIButton* m_pNextButton = { nullptr };
	CUIFont* m_pESCButtonFont= { nullptr };
	CUIFont* m_pNextButtonFont = { nullptr };


	_float m_fBoxHeight = { 275};
	_float m_fHorizontalMargin = { 200 };
	_float m_fOptionLeftOffset = { 50 };
	_float m_fOptionHeight = { 40 };
	_float m_fButtonWidth = { 80 };
	_float m_fButtonHeight = { 40 };
	_uint m_iSelectedOptionDataIndex = { 0 };
	_float4 m_f4HighlighterBorderSize = { 10,10,10,10 };
public:
	static CUINPCDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END
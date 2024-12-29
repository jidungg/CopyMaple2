#pragma once
#include "UIContainer.h"
#include "Item.h"
#include "Conversation.h"


BEGIN(Engien)
class CGameInstance;
END

BEGIN(Client)

class IQuickItem;
class CUIQuickSlotBundle;
class CUIInventory;
class CUIBar;
class CInvenSlot;
class CPlayer;
class CUIMainHUDGuage;
class CUIPlayerInfo;
class CPlayerEquipSlot;
class CPlayerDecoSlot;
class CPlayerInfoSlot;
class CUINPCDialog;
class CNPC;
class CUIQuestGuideBundle;
class CUIBundle :
    public CBase
{

	DECLARE_SINGLETON(CUIBundle)

public:
	typedef struct UIBUNDLE_DESC 
	{
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
	}BUNDLE_DESC;
protected:
	explicit CUIBundle();
	~CUIBundle() = default;

public:
	virtual HRESULT Initialize(void* pArg) ;


	void Initialize_PlayerInfo(CPlayer* pPalyer);
	void Update_Inven_Slot(_uint iIndex, CInvenSlot* pSlot);
	void Update_CastingRatio(_float fRatio);
	void Update_PlayerInfo_Slot(_uint iSlotId, CPlayerInfoSlot* pItemSlot);
	void Set_CastingBarVisible(_bool bVisible);

	CUIInventory* Get_Inventory() { return m_pInventory; }
	CUIQuickSlotBundle* Get_QuckSlotBundle() { return m_pQuickSlotBundle1; }
	void Set_QuickItem(KEY eHotKey, IQuickItem* pItem);
	void Toggle_Inventory();
	void Toggle_PlayerInfo();
	void Toggle_HUD();
	void Set_HUDActive(_bool bActive);
	void Toggle_NPCDialog();
	void Set_NPCDialogActive(_bool bActive);
	void Set_NPCDialogNPC(CNPC* pNPC);

	void Add_QuestGuide(QUEST_ID eQuestID);
	void Remove_QuestGuide(QUEST_ID eQuestID);
	void Update_QuestGuide();

	void Set_NPCDialogData(const CONVERSATION_NODE_DATA& pNode);
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

	CUIQuickSlotBundle* m_pQuickSlotBundle1 = { nullptr };
	CUIQuickSlotBundle* m_pQuickSlotBundle2 = { nullptr };
	CUIInventory* m_pInventory = { nullptr };
	CUIBar* m_pCastingBar = { nullptr };
	CUIMainHUDGuage* m_pMainHPBar = { nullptr };
	CUIPlayerInfo* m_pPlayerInfoUI = { nullptr };
	CUINPCDialog* m_pNPCDialog = { nullptr };
	CUIQuestGuideBundle* m_pQuestGuideBundle = { nullptr };
public:
	void Free() override;
};
#define UIBUNDLE CUIBundle::GetInstance()
END
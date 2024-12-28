#pragma once
#include "Interactable.h"
#include "Conversation.h"

BEGIN(Engine)
class CStateMachine;
END
BEGIN(Client)
typedef struct NPCData
{
	NPCData() = default;
	NPCData(json& js)
	{
		eNPCId = js["NPCId"];
		string str = js["Name"];
		std::copy(str.begin(), str.end(), szNPCName);
		str = js["ModelTag"];
		std::copy(str.begin(), str.end(), strModelTag);
		json& jmapAnimIdx = js["AnimIdx"];
		mapAnimIdx[N_AS_BORE] = jmapAnimIdx["BORE"].get<vector<_uint>>();
		mapAnimIdx[N_AS_IDLE] = jmapAnimIdx["IDLE"].get<vector<_uint>>();
		mapAnimIdx[N_AS_RUN] = jmapAnimIdx["RUN"].get<vector<_uint>>();
		mapAnimIdx[N_AS_WALK] = jmapAnimIdx["WALK"].get<vector<_uint>>();
		mapAnimIdx[N_AS_TALK] = jmapAnimIdx["TALK"].get<vector<_uint>>();
		mapAnimIdx[N_AS_HAPPY] = jmapAnimIdx["HAPPY"].get<vector<_uint>>();

		json& jQuest = js["Quests"];
		for (auto& pQuest : jQuest)
		{
			vecQuest.push_back(pQuest);
		}

		json& jConversation = js["ConversationNodes"];
		for (auto& pConversation : jConversation)
		{
			mapChat.insert({ pConversation["Index"], ConversationNodeData(pConversation)});
		}
	}
	NPC_ID eNPCId;
	_char szNPCName[MAX_PATH] = ("");
	_char strModelTag[MAX_PATH] = ("");
	unordered_map<NPC_ANIM_STATE, vector<_uint>> mapAnimIdx;
	map<_uint,ConversationNodeData> mapChat;
	vector<QUEST_ID> vecQuest;
}NPC_DATA;
class CCubeTerrain;
class CModelObject;

class CNPC :
    public CInteractableObject
{
public:

	enum ANIM_CONDITION
	{
		AC_ANIM_END_TRIGGER,
		AC_TALK_TRIGGER,
		AC_BORE_TRIGGER,
		AC_EMOTION_TRIGGER,
		AC_MOVE,
		AC_WALK,
		AC_RANDOM,
		AC_LAST
	};

public:
	typedef struct NPCDesc :public CGameObject::GAMEOBJECT_DESC
	{
		NPC_ID eNPCId;
		_vector vHomePos = { 10.f,1.f,10.f };
		CCubeTerrain* pCubeTerrain = { nullptr };
	}NPC_DESC;
public:
	static constexpr _tchar m_szProtoTag[] = L"Prototype_GameObject_NPC";


protected:
	explicit CNPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNPC(const CNPC& Prototype);
	virtual ~CNPC() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	
	const ConversationNodeData& Get_ConversationData(_int iIdx);
	void To_NextConversation(_uint iSelectedOptionIdx);
	void End_Conversation();
private:
	HRESULT Ready_AnimStateMachine();
	void On_StateChange(_uint iState) ;
	void On_SubStateChange(_uint iSubState) ;
	void On_AnimEnd(_uint iAnimIdx);
private:
	NPCData* m_pNPCData = { nullptr };

	CCubeTerrain* m_pCubeTerrain = { nullptr };

	CModelObject* m_pBody = { nullptr };
	CStateMachine* m_pAnimStateMachine = { nullptr };
	_float m_fBoreTimeAcc = { 0.f };
	_float m_fBoreTime= { 20.f };

	_vector m_vHomePos = { 10.f,1.f,10.f,1.f };
	_float m_fHomeRange{ 6.f };

	_float m_fRandom = { 0.f };
	_bool m_bMove = { false };
	_bool m_bWalk = { false };
	_int m_iConversationIndex = { -1 };
	QUEST_ID m_eCurrentConversationQuest = { QUEST_ID::LAST };
public:
	static CNPC* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


	// CInteractableObject을(를) 통해 상속됨
	void Interact(CPlayer* pActor) override;

	_bool Is_InteractionPossible(CPlayer* pActor) override;

};

END
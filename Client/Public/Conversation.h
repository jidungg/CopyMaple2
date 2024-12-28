#pragma once
#include "Base.h"

BEGIN(Client)


enum class CHAT_OPT_TYPE
{
	NORMAL,
	QUEST,
	LAST
};
NLOHMANN_JSON_SERIALIZE_ENUM(CHAT_OPT_TYPE, {
	{CHAT_OPT_TYPE::NORMAL, "CHAT_OPT_NORMAL"},
	{CHAT_OPT_TYPE::QUEST, "CHAT_OPT_QUEST"},
	{CHAT_OPT_TYPE::LAST, "CHAT_OPT_LAST"}
	});
//스크립트 리스트
typedef struct ChatPortraitData
{
	ChatPortraitData() = default;
	ChatPortraitData(json& js);

	_tchar szPortraitTag[MAX_PATH] = TEXT("");
	_bool bGray = { false };
};
typedef struct ChatOptionData
{
	ChatOptionData() = default;
	ChatOptionData(json& js);
	
	CHAT_OPT_TYPE eOptType = CHAT_OPT_TYPE::NORMAL;
	QUEST_ID eQuestID = QUEST_ID::LAST;
	_tchar szText[MAX_PATH] = TEXT("");
	_int iNextNode = { -1 };
}CHAT_OPTION_DATA;
typedef struct ConversationNodeData 
{
	ConversationNodeData() = default;
	ConversationNodeData(json& js);
	
	vector<ChatPortraitData> vecPortrait;

	_tchar szSpeakerName[MAX_PATH] = TEXT("");
	_tchar szScript[MAX_PATH] = TEXT("");
	vector<ChatOptionData> vecOption;
	_int iNextNode = { -1 };
}CONVERSATION_NODE_DATA;

END
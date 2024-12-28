#include "stdafx.h"
#include "Conversation.h"
#include "Engine_Utility.h"

ConversationNodeData::ConversationNodeData(json& js)
{
	string str = js["Script"];

	wstring wstr = CEngineUtility::ConvertStringToWString(str);
	std::copy(wstr.begin(), wstr.end(), szScript);
	json& jPortraits = js["Portraits"];

	for (auto& jPortrait : jPortraits)
	{
		vecPortrait.push_back(ChatPortraitData(jPortrait));
	}

	str = js["Speaker"];
	wstr = CEngineUtility::ConvertStringToWString(str);
	std::copy(wstr.begin(), wstr.end(), szSpeakerName);

	//옵션이 없으면 iNextNode를 읽는다.
	json& jOptions = js["Options"];
	if (jOptions.size() <= 0)
		iNextNode = js["Next"];
	else
	{
		for (auto& jOpt : jOptions)
		{
			vecOption.push_back(ChatOptionData(jOpt));
		}
	}
}

ChatPortraitData::ChatPortraitData(json& js)
{
	string str = js["Tag"];
	wstring wstr = CEngineUtility::ConvertStringToWString(str);
	std::copy(wstr.begin(), wstr.end(), szPortraitTag);
	bGray = js["Gray"];
}

ChatOptionData::ChatOptionData(json& js)
{
	eOptType = js["Type"];
	switch (eOptType)
	{
	case Client::CHAT_OPT_TYPE::NORMAL:
	{
		string str = js["Text"];
		wstring wstr = CEngineUtility::ConvertStringToWString(str);
		std::copy(wstr.begin(), wstr.end(), szText);
		iNextNode = js["Next"];
		break;
	}
	case Client::CHAT_OPT_TYPE::QUEST:
		eQuestID = js["QuestID"];
		break;
	case Client::CHAT_OPT_TYPE::LAST:
	default:
		break;
	}

}
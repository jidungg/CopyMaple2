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

json ConversationNodeData::To_Json()
{
	json jResult;
	jResult["Script"] = CEngineUtility::ConvertWStringToString(szScript);
	jResult["Speaker"] = CEngineUtility::ConvertWStringToString(szSpeakerName);
	jResult["Next"] = iNextNode;
	json jPortraits;
	for (auto& portrait : vecPortrait)
	{
		jPortraits.push_back(portrait.To_Json());
	}
	jResult["Portraits"] = jPortraits;
	json jOptions;
	for (auto& opt : vecOption)
	{
		jOptions.push_back(opt.To_Json());
	}
	jResult["Options"] = jOptions;

	return jResult;
}

ChatPortraitData::ChatPortraitData(json& js)
{
	string str = js["Tag"];
	wstring wstr = CEngineUtility::ConvertStringToWString(str);
	std::copy(wstr.begin(), wstr.end(), szPortraitTag);
	bGray = js["Gray"];
}

json ChatPortraitData::To_Json()
{
	json jResult;
	jResult["Tag"] = CEngineUtility::ConvertWStringToString(szPortraitTag);
	jResult["Gray"] = bGray;
	return jResult;
}

ChatOptionData::ChatOptionData(json& js)
{
	eOptType = js["Type"];
	switch (eOptType)
	{
	case Client::CHAT_OPT_TYPE::NORMAL:
	case Client::CHAT_OPT_TYPE::ABANDON_QUEST:
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

json ChatOptionData::To_Json()
{
	json jResult;
	jResult["Type"] = eOptType;
	switch (eOptType)
	{
	case Client::CHAT_OPT_TYPE::ABANDON_QUEST:
	case Client::CHAT_OPT_TYPE::NORMAL:
		jResult["Text"] = CEngineUtility::ConvertWStringToString(szText);
		jResult["Next"] = iNextNode;
		break;
	case Client::CHAT_OPT_TYPE::QUEST:
		jResult["QuestID"] = eQuestID;
		break;
	case Client::CHAT_OPT_TYPE::LAST:
	default:
		break;
	}
	return jResult;
}

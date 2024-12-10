#include "stdafx.h"
#include "Item.h"
#include "RenderTarget.h"
#include "RenderTarget_Manager.h"
#include "GameInstance.h"
//

BuildItemData::BuildItemData(string strFileName, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	eITemType = ITEM_TYPE::BUILD;

	string strTmp = strFileName.substr(strFileName.find_first_of("_") + 1);
	string strGroup = strTmp.substr(0, strTmp.find_first_of("_"));
	if (strGroup == "ground")
		eBuildType = BUILD_ITEM_TYPE::GROUND;
	else if (strGroup == "fi")
	{
		strTmp = strTmp.substr(strTmp.find_first_of("_") + 1);
		string strType = strTmp.substr(0, strTmp.find_first_of("_"));
		if (strType == "cubric")
			eBuildType = BUILD_ITEM_TYPE::CUBRIC;
		else if (strType == "funct")
			eBuildType = BUILD_ITEM_TYPE::FUNCT;
		else if (strType == "nature")
			eBuildType = BUILD_ITEM_TYPE::NATURE;
		else if (strType == "prop")
			eBuildType = BUILD_ITEM_TYPE::PROP;
		else if (strType == "struc")
			eBuildType = BUILD_ITEM_TYPE::STRUC;
		else
			assert(false);
	}
	switch (eBuildType)
	{
	case Client::BUILD_ITEM_TYPE::GROUND:
	case Client::BUILD_ITEM_TYPE::CUBRIC:
	case Client::BUILD_ITEM_TYPE::STRUC:
		eBlockType = BUILD_ITEM_BLOCK_TYPE::BLOCK;
		break;
	default:
		eBlockType = BUILD_ITEM_BLOCK_TYPE::NON_BLOCK;
		break;
	}
	iPrice = 0;
	eItemGrade = ITEM_GRADE::NORMAL;
	std::copy(strFileName.begin(), strFileName.end(), strItemName);
	std::copy(strFileName.begin(), strFileName.end(), strItemDesc);
	std::copy(strFileName.begin(), strFileName.end(), strModelTag);
	string strIconTag = strFileName.substr(0, strFileName.find_first_of("."));
	strIconTag += "_icon";
	std::copy(strIconTag.begin(), strIconTag.end(), strIconImageTag);
}

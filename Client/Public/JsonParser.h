#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)

class CJsonParser :
    public CBase
{
public:
	static HRESULT ReadJsonFile(const _char* strJsonFilePath, json* pOut)
	{
		std::ifstream input_file(strJsonFilePath);


		if (!input_file.is_open()) {
			// Handle error
			return E_FAIL;
		}
		// JSON 객체로 변환
		input_file >> *pOut;

		// 파일 닫기
		input_file.close();
		return S_OK;
	}
	static HRESULT SaveJsonFile(const _char* strJsonFilePath, const json& pJson)
	{


		std::ofstream file(strJsonFilePath);
		if (file.is_open()) {
			file << pJson.dump(4);  // 들여쓰기를 4칸으로 설정
			file.close();

		}
		else {
			return E_FAIL;
		}
		return S_OK;
	}
};

END
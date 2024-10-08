#pragma once
#include "Base.h"


BEGIN(Engine)

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
		// JSON ��ü�� ��ȯ
		input_file >> *pOut;

		// ���� �ݱ�
		input_file.close();
		return S_OK;
	}
};

END
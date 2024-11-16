#include "pch.h"
#include "CNifToBinary.h"

CNifToBinary::CNifToBinary()
{
}

CNifToBinary::~CNifToBinary()
{
}

HRESULT CNifToBinary::NifToBinary(const string& inFilePath)
{
    auto root = ReadNifTree(inFilePath);

    if (root) {
        std::cout << "NIF file loaded successfully!" << std::endl;
    }

	return S_OK;
}

#include "pch.h"
#include "CNIFToBinary.h"
#include "niflib/niflib.h"
#include "niflib/nif_io.h"
#include "niflib/obj/NiObject.h"

using namespace Niflib;
CNIFToBinary::CNIFToBinary()
{
}
CNIFToBinary::~CNIFToBinary()
{
}
HRESULT CNIFToBinary::NIFToBinary(const string& inFilePath)
{
    //auto root = ReadNifTree(inFilePath);

    //if (root) {
    //    std::cout << "NIF file loaded successfully!" << std::endl;
    //}

	return S_OK;
}

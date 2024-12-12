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

	unsigned int iVersion = Niflib::GetNifVersion(inFilePath);
  //  if(false == Niflib::IsSupportedVersion(iVersion))
  //  {
		//return E_FAIL;
  //  }
    auto root = ReadNifTree(inFilePath);
    if (root) {
        std::cout << "NIF file loaded successfully!" << std::endl;
    }


    auto node = DynamicCast<NiNode>(root);
    if (node) {
        for (const auto& child : node->GetChildren()) {
            std::cout << "Child Node Type: " << child->GetType().GetTypeName() << std::endl;
        }
    }

    auto mesh = DynamicCast<NiTriShape>(node);
    if (mesh) {
        auto data = mesh->GetData();
        if (data) {
            std::cout << "Vertex Count: " << data->GetVertexCount() << std::endl;
            std::cout << "Face Count: " << data->GetCenter() << std::endl;
        }
    }
	return S_OK;
}

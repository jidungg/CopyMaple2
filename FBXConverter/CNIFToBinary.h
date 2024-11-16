#pragma once

class CNIFToBinary
{
public:
	CNIFToBinary();
	~CNIFToBinary();

	HRESULT NIFToBinary(const string& inFilePath);
private:
	//HRESULT Write_Mesh(const NiTriShape* pNIMesh, ofstream& outFile);
	//HRESULT Write_Material(const NiMaterialProperty* pNIMaterial, ofstream& outFile);
	//HRESULT Write_Bone(const NiNode* pNIBone, ofstream& outFile);
	//HRESULT Write_Animation(const NiControllerSequence* pNIAnim, ofstream& outFile);


};


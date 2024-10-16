#pragma once

class CModelConverter
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

public:
	CModelConverter();
	~CModelConverter();

	HRESULT FbxToBianry(const string& inFilePath);
	//HRESULT BinaryToModel(const string& file, TYPE type = TYPE_NONANIM);

private:
	HRESULT Write_Meshe(const aiMesh* pAIMesh, ofstream& outFile);
	HRESULT Write_Material(const aiMaterial* pAIMaterial, ofstream& outFile);
	HRESULT Write_Bone(const aiNode* pAIBone, ofstream& outFile);
private:
	Assimp::Importer			m_Importer;
	const aiScene* m_pAIScene = { nullptr };
	bool m_bAnim = { false };
};


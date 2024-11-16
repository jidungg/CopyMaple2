#pragma once


typedef struct
{
	XMFLOAT3		vScale;
	XMFLOAT4		vRotation;
	XMFLOAT3		vPosition;
	float			fKeyFramePosition;
}KEYFRAME;
class CFbxToBinary
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

public:
	CFbxToBinary();
	~CFbxToBinary();

	HRESULT FbxToBinary(const string& inFilePath);
	//HRESULT BinaryToModel(const string& file, TYPE type = TYPE_NONANIM);

private:
	HRESULT Write_Meshe(const aiMesh* pAIMesh, ofstream& outFile);
	HRESULT Write_Material(const aiMaterial* pAIMaterial, ofstream& outFile);
	HRESULT Write_Bone(const aiNode* pAIBone, ofstream& outFile);
	HRESULT Write_Animation(const aiAnimation* pAIAnim, ofstream& outFile);
private:
	Assimp::Importer			m_Importer;
	const aiScene* m_pAIScene = { nullptr };
	bool m_bAnim = { false };
};

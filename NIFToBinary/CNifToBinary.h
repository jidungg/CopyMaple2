#pragma once


typedef struct
{
	XMFLOAT3		vScale;
	XMFLOAT4		vRotation;
	XMFLOAT3		vPosition;
	float			fKeyFramePosition;
}TRANSFORM_KEYFRAME;
class CNifToBinary
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

public:
	CNifToBinary();
	~CNifToBinary();

	HRESULT NifToBinary(const string& inFilePath);
	//HRESULT BinaryToModel(const string& file, TYPE type = TYPE_NONANIM);

private:
	HRESULT Write_Meshe(const Ref<NiObject>& obj, ofstream& outFile);
	HRESULT Write_Material(const Ref<NiObject>& obj, ofstream& outFile);
	HRESULT Write_Bone(const Ref<NiObject>& obj, ofstream& outFile);
	HRESULT Write_Animation(const Ref<NiObject>& obj, ofstream& outFile);
private:

};

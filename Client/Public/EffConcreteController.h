#pragma once
#include "EffController.h"
BEGIN(Client)

typedef struct MaterialColorKeyframe : public KEYFRAME
{
	_float3		vAmbient = { 0,0,0 };
	_float3		vDiffuse = { 0,0,0 };
	_float3		vSpecular = { 0,0,0 };
	_float3		vEmissive = { 0,0,0 };
}MATCOLOR_KEYFRAME;

class CEffAlphaController :
    public CEffController
{
protected:
	CEffAlphaController();
	virtual ~CEffAlphaController() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream& inFile, const class CEffModel* pModel) override;
	virtual _bool Update_Controller(_float fCurrentTrackPos) override;

private:
	vector<FLOAT_KEYFRAME> m_vecKeyFrame;
public:
	static CEffAlphaController* Create(ifstream& inFile, const class CEffModel* pModel);
	virtual CEffAlphaController* Clone();
};
class CEffTextureTransfromController :
	public CEffController
{
protected:
	CEffTextureTransfromController();
	virtual ~CEffTextureTransfromController() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream& inFile, const class CEffModel* pModel) override;
	virtual _bool Update_Controller(_float fCurrentTrackPos) override;

private:
	EFF_TEX_TYPE m_eTexSlot = EFF_TEX_TYPE::TT_LAST;
	EFF_TEX_OPERATION_TYPE m_eTexOperation = EFF_TEX_OPERATION_TYPE::TO_LAST;
	vector< FLOAT_KEYFRAME> m_vecKeyFrame;
public:
	static CEffTextureTransfromController* Create(ifstream& inFile, const class CEffModel* pModel);
	virtual CEffTextureTransfromController* Clone();
};
class CEffTransformController :
	public CEffController
{
protected:
	CEffTransformController();
	virtual ~CEffTransformController() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream& inFile, const class CEffModel* pModel) override;
	virtual _bool Update_Controller(_float fCurrentTrackPos)  override;

private:
	vector<TRANSFORM_KEYFRAME> m_vecKeyFrame;
public:
	static CEffTransformController* Create(ifstream& inFile, const class CEffModel* pModel);
	virtual CEffTransformController* Clone();
};
class CEffMaterialColorController :
	public CEffController
{
protected:
	CEffMaterialColorController();
	virtual ~CEffMaterialColorController() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream& inFile, const class CEffModel* pModel) override;
	virtual _bool Update_Controller(_float fCurrentTrackPos) override;

private:
	vector< MATCOLOR_KEYFRAME> m_vecKeyFrame;
public:
	static CEffMaterialColorController* Create(ifstream& inFile, const class CEffModel* pModel);
	virtual CEffMaterialColorController* Clone();
};
END
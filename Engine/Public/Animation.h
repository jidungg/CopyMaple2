#pragma once

#include "Base.h"



BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(ifstream& inFile, const class CModel* pModel);
	bool Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta);
	
	void Set_Loop(bool bLoop) { m_bLoop = bLoop; }

private:
	_char					m_szName[MAX_PATH] = {};
	_uint					m_iNumChannels = {};
	vector<class CChannel*> m_Channels;

	_float					m_fDuration = {};
	_float					m_fTickPerSecond = {};
	_float					m_fCurrentTrackPosition = {};

	bool						m_bLoop = {false};
	
	vector<_uint>			m_CurrentKeyFrameIndices;
public:
	static CAnimation* Create(ifstream& inFile, const class CModel* pModel);
	virtual CAnimation* Clone();
	virtual void Free() override;
};

END
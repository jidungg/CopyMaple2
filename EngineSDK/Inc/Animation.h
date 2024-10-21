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
	bool Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, CAnimation* pNextAnim);
	
	void Set_Loop(bool bLoop) { m_bLoop = bLoop; }
	void Reset_CurrentTrackPosition() { m_fCurrentTrackPosition = 0.f; }
	float Get_CurrentTrackPosition() { return m_fCurrentTrackPosition; }
	float Get_AnimTime() { return m_fDuration; }

private:
	_char					m_szName[MAX_PATH] = {};
	_uint					m_iNumChannels = {};
	vector<class CChannel*> m_Channels;

	_float					m_fDuration = {};
	_float					m_fTickPerSecond = {};
	_float					m_fCurrentTrackPosition = {};
	_float					m_fAnimTransitionTime= {0.2f};

	bool						m_bLoop = {false};
	
	//Cahnnel마다 현재 KeyFrameIndex를 저장하는 배열
	vector<_uint>			m_CurrentKeyFrameIndices;
public:
	static CAnimation* Create(ifstream& inFile, const class CModel* pModel);
	virtual CAnimation* Clone();
	virtual void Free() override;
};

END
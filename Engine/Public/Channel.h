#pragma once

#include "Base.h"

/* 0. �� ���� �Ҽӵ� �ִϸ��̼��� ǥ���ϱ����ؼ� ���� � ����(KeyFrame)(���)���� �����°�?! �� ���� ������ ������. */
/* 1. ���� ����ð��� �´� ���� ��������� ������.  */
/* 2. ���� �̸��� ���� ä���� �ټ� ������ �� �ִ�?! */

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(ifstream& inFile, const class CModel* pModel);
	void Update_TransformationMatrix(_float fCurrentTrackPosition, _uint* pKeyFrameIndex, const vector<class CBone*>& Bones);

private:
	_char						m_szName[MAX_PATH] = {};
	_uint						m_iNumKeyFrames = {};
	vector<KEYFRAME>			m_KeyFrames;
	_uint						m_iBoneIndex = {};

public:
	static CChannel* Create(ifstream& inFile, const class CModel* pModel);
	virtual void Free() override;
};

END
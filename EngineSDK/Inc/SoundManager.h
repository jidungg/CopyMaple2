#pragma once

#include "Base.h"


BEGIN(Engine)

class CSound;

class CSoundManager : public CBase
{

public:
	enum SFX_TYPE
	{
		SFX_TEST,
		SFX_END

	};

	enum BGM_TYPE
	{
		BGM_TEST,
		BGM_END

	};
private:
	CSoundManager();
	virtual ~CSoundManager();

public:
	HRESULT CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, _Out_ LPDIRECTSOUNDBUFFER* ppDSBuffer, _Pre_null_ LPUNKNOWN pUnkOuter);
	HRESULT			Ready_SoundMgr(HWND hWnd, _uint iLevelCount);
	LPDIRECTSOUND8	Get_SoundDevice() { return m_pSound; }

	HRESULT			Load_BGM(_uint iLevelID, const wstring& strKey, const wstring& strPath);
	HRESULT			Load_SFX(_uint iLevelID, const wstring& strKey, const wstring& strPath);
	void					Clear(_uint iLevelIndex);

	void  Start_BGM(_uint iLevelID, const wstring& strBGM, _float fStartPos = 0, _bool _bRepeat = true);
	void			End_BGM();
	void			Set_BGMVolume(float _fVolume);
	float			Get_BGMVolume();
	CSound* Start_EffectPlay(_uint iLevelID, const wstring& strSFX, _float fStartPos = 0, _bool _bRepeat = false);
	CSound* Start_EffectPlay_Random(_uint iLevelID, const wstring& strSFX,_uint iStart, _uint iEnd ,_float fStartPos = 0, _bool _bRepeat = false);
	void			Stop_EffectPlay(_uint iLevelID, const wstring& strSFX);

	CSound* Find_BGM(_uint iLevelID, const wstring& strBGM);
	CSound* Find_SFX(_uint iLevelID, const wstring& strSFX);

	float					Get_SFXPosition(_uint iLevelID, const wstring& strKey);
	bool					Is_SFXPlaying(_uint iLevelID, const wstring& strKey);

	float					Get_BGMPosition(_uint iLevelID, const wstring& strKey);
	bool					Is_BGMPlaying(_uint iLevelID, const wstring& strKey);

	CSound* GetCurBGM() { return m_CurBGM; }
	CSound* Get_SFX(_uint iLevelID, const wstring& strKey) { return m_mapSFX[iLevelID][strKey]; }
private:
	LPDIRECTSOUND8	m_pSound;	// 사운드카드 대표 객체
	map<wstring, CSound*>* m_mapBGM;
	map<wstring, CSound*>* m_mapSFX;
	_uint m_iLevelCount = { 0 };
	CSound* m_CurBGM;


public:
	static CSoundManager* Create();

	virtual void Free();
};


#define SOUND_MGR CSoundManager::GetInstance()

END
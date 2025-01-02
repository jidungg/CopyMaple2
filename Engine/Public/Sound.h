#pragma once


#include "Base.h"
#include "SoundManager.h"
BEGIN(Engine)

class ENGINE_DLL CSound : public CBase
{
private:
	explicit CSound();
	explicit CSound(const wstring& _strName);
	virtual ~CSound();
public:
	HRESULT		Load(const wstring& _strPath);

	// 일반 재생
	void		Play(_float fStartPos = 0, bool _bLoop = false);

	// BGM 으로 재생
	void		PlayToBGM(float _StartPosition, bool _bLoop = false);
	void		PlayToSampleBGM();

	void		Stop(bool _bReset = false);

	// 볼륨 범위 (0 ~ 100)
	void		SetVolume(float _fVolume);
	int			GetVolume() { return m_iVolume; };

	// 음악파일 위치 조정
	void		SetPosition(float _fPosition); // 0 ~ 100 사이

	float GetPosition();
	bool				Is_Playing();


	wstring		GetKey() { return m_strKey; }
	wstring		GetPath() { return m_strRelativePath; }
	void		SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void		SetPath(const wstring& _strPath) { m_strRelativePath = _strPath; }

	static		CSound* Create(const wstring& _strPath);

private:
	HRESULT		LoadWaveSound(const wstring& _strPath);
	int			GetDecibel(float _fVolume);


private:
	LPDIRECTSOUNDBUFFER		m_pSoundBuffer;
	DSBUFFERDESC			m_tBuffInfo;
	int						m_iVolume;

	wstring					m_strKey; // 리소스 키
	wstring					m_strRelativePath; // 리소스 상대경로

	class CGameInstance* m_pGameInstance = { nullptr };
private:
	virtual void Free();
};

END
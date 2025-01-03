#include "SoundManager.h"
#include "Sound.h" 


CSoundManager::CSoundManager()
	: m_pSound(nullptr)
	, m_CurBGM(nullptr)
{
}

CSoundManager::~CSoundManager()
{
}


HRESULT CSoundManager::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER* ppDSBuffer, LPUNKNOWN pUnkOuter)
{
	return m_pSound->CreateSoundBuffer(pcDSBufferDesc, ppDSBuffer, pUnkOuter);
}

HRESULT CSoundManager::Ready_SoundMgr(HWND hWnd, _uint iLevelCount)
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pSound, NULL)))
		return E_FAIL;

	// 사운드 디바이스 협조레벨 설정.
	if (FAILED(m_pSound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE)))
		return E_FAIL; // Flag값 정리

	m_mapBGM = new map<wstring, CSound*>[iLevelCount];
	m_mapSFX = new map<wstring, CSound*>[iLevelCount];
		
	m_iLevelCount = iLevelCount;
	return S_OK;
}


void CSoundManager::Start_BGM(_uint iLevelID, const wstring& strBGM, _float fStartPos, _bool _bRepeat)
{
	if (nullptr != m_CurBGM)
	{
		m_CurBGM->Stop();
		Safe_Release(m_CurBGM);
	}
	m_CurBGM = Find_BGM(iLevelID, strBGM);
	if (nullptr == m_CurBGM) return;
	Safe_AddRef(m_CurBGM);

	m_CurBGM->PlayToBGM(fStartPos, _bRepeat);
}

void CSoundManager::End_BGM()
{
	if (nullptr != m_CurBGM)
		m_CurBGM->Stop();
}

void CSoundManager::Set_BGMVolume(float _fVolume)
{
	m_CurBGM->SetVolume(_fVolume);
}

float CSoundManager::Get_BGMVolume()
{
	return (float)m_CurBGM->GetVolume();
}

CSound* CSoundManager::Start_EffectPlay(_uint iLevelID, const wstring& strSFX, _float fStartPos, _bool _bRepeat)
{
	CSound* pSound = Find_SFX(iLevelID, strSFX);
	if (pSound == nullptr)return nullptr ;
	if (fStartPos <= 0)
	{
		m_listDelaySound.push_back(pSound);
	}
	pSound->Stop();
	pSound->Play(fStartPos, _bRepeat);
	return pSound;
}

CSound* CSoundManager::Start_EffectPlay_Random(_uint iLevelID, const wstring& strSFX, _uint iStart, _uint iEnd, _float fStartPos, _bool _bRepeat)
{
	_uint irand = rand() % (iEnd - iStart +1)+iStart;
	wchar_t result[MAX_PATH];
	swprintf(result, MAX_PATH, strSFX.c_str(), irand);
	CSound* pSound =Start_EffectPlay(iLevelID, result, fStartPos,_bRepeat);
	return pSound;
}


void CSoundManager::Stop_EffectPlay(_uint iLevelID, const wstring& strSFX)
{
	CSound* pSound = Find_SFX(iLevelID,strSFX);

	if(nullptr ==pSound)return;
	pSound->Stop();
}


CSound* CSoundManager::Find_BGM(_uint iLevelID, const wstring& strBGM)
{
	CSound* pBgm = nullptr;
	auto iter = m_mapBGM[iLevelID].find(strBGM);
	if (iter != m_mapBGM[iLevelID].end())
		pBgm = iter->second;
	return pBgm;
}

CSound* CSoundManager::Find_SFX(_uint iLevelID, const wstring& strSFX)
{
	CSound* pSfx = nullptr;
	auto iter = m_mapSFX[iLevelID].find(strSFX);
	if (iter != m_mapSFX[iLevelID].end())
		pSfx = iter->second;
	return pSfx;
}

HRESULT CSoundManager::Load_BGM(_uint iLevelID, const wstring& strKey, const wstring& strPath)
{
	CSound* pSnd = CSound::Create(strPath);
	if(nullptr == pSnd) return E_FAIL;
	m_mapBGM[iLevelID].try_emplace(strKey, pSnd);
	return S_OK;
}

HRESULT CSoundManager::Load_SFX(_uint iLevelID, const wstring& strKey, const wstring& strPath)
{
	CSound* pSnd = CSound::Create(strPath);
	if (nullptr == pSnd) return E_FAIL;
	m_mapSFX[iLevelID].try_emplace(strKey, pSnd);
	return S_OK;
}

void CSoundManager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iLevelCount)
		return;
	if (iLevelIndex == 0)
		return;
	for (auto& Pair : m_mapBGM[iLevelIndex])
		Safe_Release(Pair.second);
	for (auto& Pair : m_mapSFX[iLevelIndex])
		Safe_Release(Pair.second);
	m_mapBGM[iLevelIndex].clear();
	m_mapSFX[iLevelIndex].clear();
}

float CSoundManager::Get_SFXPosition(_uint iLevelID, const wstring& strKey)
{
	if (m_mapSFX[iLevelID].find(strKey) == m_mapSFX[iLevelID].end())
		return 0.f;
	return m_mapSFX[iLevelID][strKey]->GetPosition();
}

bool CSoundManager::Is_SFXPlaying(_uint iLevelID, const wstring& strKey)
{
	if (m_mapSFX[iLevelID].find(strKey) == m_mapSFX[iLevelID].end())return false;
	return m_mapSFX[iLevelID][strKey]->Is_Playing();
}

float CSoundManager::Get_BGMPosition(_uint iLevelID, const wstring& strKey)
{
	if (m_mapSFX[iLevelID].find(strKey) == m_mapSFX[iLevelID].end())return 0.f;
	return m_mapBGM[iLevelID][strKey]->GetPosition();
}

bool CSoundManager::Is_BGMPlaying(_uint iLevelID, const wstring& strKey)
{
	if (m_mapSFX[iLevelID].find(strKey) == m_mapSFX[iLevelID].end())return false;
	return m_mapBGM[iLevelID][strKey]->Is_Playing();
}


CSoundManager* CSoundManager::Create()

{
	return new CSoundManager();
}

void CSoundManager::Free()
{
	for (_uint i = 0; i < m_iLevelCount; ++i)
	{
		for (auto& pBGM : m_mapBGM[i])
		{
			Safe_Release(pBGM.second);
		}
		for (auto& pSFX : m_mapSFX[i])
		{
			Safe_Release(pSFX.second);
		}
	}
	delete[] m_mapBGM;
	delete[] m_mapSFX;
	Safe_Release(m_CurBGM);
}

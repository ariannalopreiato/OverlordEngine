#include "stdafx.h"
#include "GameSoundManager.h"

GameSoundManager::GameSoundManager()
{
	Initialize();
}

void GameSoundManager::SetUp()
{
	m_pSoundSystem = SoundManager::Get()->GetSystem();
}

void GameSoundManager::AddSound(Sound soundID, const std::string& path, bool isLooping)
{
	//check that the sound is not already stored
	if (m_SoundMap.contains(soundID))
		return;

	FMOD::Sound* pSound{};
	FMOD_RESULT result{};

	if(isLooping)
		result = m_pSoundSystem->createStream(path.c_str(), FMOD_DEFAULT, nullptr, &pSound);
	else
		result = m_pSoundSystem->createStream(path.c_str(), FMOD_LOOP_OFF, nullptr, &pSound);

	m_SoundMap.insert({ soundID, pSound });
}

void GameSoundManager::Play2DSound(Sound soundID, bool isMusic)
{
	if (!m_SoundMap.contains(soundID))
	{
		std::cout << "the file doesn't exist " << (int)soundID << std::endl;
		return;
	}

	FMOD::Sound* pSound = m_SoundMap.at(soundID);

	if (!isMusic)
	{
		m_pSoundSystem->playSound(pSound, nullptr, false, &m_pSFX);
	}
	else
	{
		m_pSoundSystem->playSound(pSound, nullptr, false, &m_pMusic);
	}
}

void GameSoundManager::Stop2DSound(Sound soundID, bool isMusic)
{
	if (!m_SoundMap.contains(soundID))
	{
		std::cout << "the file doesn't exist " << (int)soundID << std::endl;
		return;
	}

	FMOD::Sound* pSound = m_SoundMap.at(soundID);

	if (!isMusic)
	{
		m_pSoundSystem->playSound(pSound, nullptr, true, &m_pSFX);
	}
	else
	{
		m_pSoundSystem->playSound(pSound, nullptr, true, &m_pMusic);
	}
}

#pragma once
#include "Utils/Singleton.h"
class GameSoundManager final : public Singleton<GameSoundManager>
{
public:

	enum class Sound
	{
		TitleMusic,
		WindfallIsland,
		StartGame,
		PauseMenuOpen,
		PauseMenuContinue,
		PauseMenuToMainMenu,
		GameOver,
		Hop,
		Hup,
		Yup,
		Rupee
	};

	GameSoundManager(const GameSoundManager& other) = delete;
	GameSoundManager(GameSoundManager&& other) noexcept = delete;
	GameSoundManager& operator=(const GameSoundManager& other) = delete;
	GameSoundManager& operator=(GameSoundManager&& other) noexcept = delete;
	void AddSound(Sound soundID, const std::string& path, bool isLooping = true);

	void Play2DSound(Sound soundID, bool isMusic = false);
	void Stop2DSound(Sound soundID, bool isMusic = false);

	void SetUp();

protected:
	void Initialize() override {};

private:
	friend Singleton<GameSoundManager>;
	GameSoundManager();
	~GameSoundManager() = default;


	FMOD::Channel* m_pMusic{};
	FMOD::Channel* m_pSFX{};
	FMOD::System* m_pSoundSystem{};
	std::unordered_map<Sound, FMOD::Sound*> m_SoundMap{};
};

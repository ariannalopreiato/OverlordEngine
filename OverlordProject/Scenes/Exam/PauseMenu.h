#pragma once
class PauseMenu : public GameScene
{
public:
	PauseMenu() :GameScene(L"Pause Menu") {}
	~PauseMenu() override = default;

	PauseMenu(const PauseMenu& other) = delete;
	PauseMenu(PauseMenu&& other) noexcept = delete;
	PauseMenu& operator=(const PauseMenu& other) = delete;
	PauseMenu& operator=(PauseMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	GameObject* m_pBackground{};
	GameObject* m_MainMenu{};
	GameObject* m_Restart{};
	GameObject* m_EndButton{};

	FMOD::Channel* m_pOpen{};
	FMOD::Channel* m_Continue{};
	FMOD::Channel* m_ToMainMenu{};
	bool m_IsPlaying{ false };
};
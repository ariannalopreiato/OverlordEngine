#pragma once
class WinScreen : public GameScene
{
public:
	WinScreen() :GameScene(L"Win Screen") {}
	~WinScreen() override = default;

	WinScreen(const WinScreen& other) = delete;
	WinScreen(WinScreen&& other) noexcept = delete;
	WinScreen& operator=(const WinScreen& other) = delete;
	WinScreen& operator=(WinScreen&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	GameObject* m_pBackground{};
	GameObject* m_MainMenu{};
	GameObject* m_TryAgain{};
};
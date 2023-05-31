#pragma once
class MainMenu : public GameScene
{
public:
	MainMenu() :GameScene(L"Main Menu") {}
	~MainMenu() override = default;

	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) noexcept = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	GameObject* m_pSprite{};
	GameObject* m_StartButton{};
	GameObject* m_EndButton{};
};
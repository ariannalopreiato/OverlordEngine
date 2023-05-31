#pragma once
class GameOver : public GameScene
{
public:
	GameOver() :GameScene(L"Game Over") {}
	~GameOver() override = default;

	GameOver(const GameOver& other) = delete;
	GameOver(GameOver&& other) noexcept = delete;
	GameOver& operator=(const GameOver& other) = delete;
	GameOver& operator=(GameOver&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	GameObject* m_pBackground{};
	GameObject* m_MainMenu{};
	GameObject* m_TryAgain{};
};
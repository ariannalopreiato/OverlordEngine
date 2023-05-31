#pragma once

class Character;
class CollectiblePrefab;
class Ladder;
class SpriteFont;
class TimerPrefab;
class LifeManager;
class ExamScene : public GameScene
{
public:
	ExamScene() : GameScene(L"2DAE07 - Lopreiato Arianna - Exam 2023") {}
	~ExamScene() override = default;

	ExamScene(const ExamScene& other) = delete;
	ExamScene(ExamScene&& other) noexcept = delete;
	ExamScene& operator=(const ExamScene& other) = delete;
	ExamScene& operator=(ExamScene&& other) noexcept = delete;

	void IncreasePoints();
	bool CheckGameWon();

protected:
	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Draw() override;
	void OnGUI() override;

private:
	void InitializePlayer();
	void InitializeCollectibles();
	//void PositionLaddersTrigger();
	void Reset();
	void LoadLevel();
	void CheckForCollectibles();
	void DisplayPoints();

	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterRoll,
		CharacterClimb,
		MoveCameraLeft,
		MoveCameraRight
	};

	SpriteFont* m_pFont;
	Character* m_pPlayer{};
	std::vector<CollectiblePrefab*> m_Collectibles{};
	int m_CurrentPoints{ 0 };
	int m_TotalPoints{};
	float m_GameDuration;
	float m_CurrentTime;
	TimerPrefab* m_Timer{};
	LifeManager* m_LifeManager{};
};
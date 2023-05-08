#pragma once

class Character;
class CollectiblePrefab;
class Ladder;
class ExamScene : public GameScene
{
public:
	ExamScene() : GameScene(L"2DAE07 - Lopreiato Arianna - Exam 2023") {}
	~ExamScene() override = default;

	ExamScene(const ExamScene& other) = delete;
	ExamScene(ExamScene&& other) noexcept = delete;
	ExamScene& operator=(const ExamScene& other) = delete;
	ExamScene& operator=(ExamScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	void InitializePlayer();
	void InitializeCollectibles();
	void PositionLaddersTrigger();
	void Reset();

	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterRoll,
		CharacterClimb
	};

	Character* m_pPlayer{};
	std::vector<CollectiblePrefab*> m_Collectibles;
	std::vector<Ladder*> m_Ladders;
};
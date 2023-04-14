#pragma once

class Player;
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
	void Reset();

	Player* m_pPlayer{};
};
#pragma once
class PongScene : public GameScene
{
public:
	PongScene() : GameScene(L"Pong Scene") {}
	~PongScene() override = default;

	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	void InitializeBall();
	void InitializePlayer(float x, float y, float z, GameObject*& player);
	void InitializeTriggers();

	void PlayerOneMovement();
	void PlayerTwoMovement();
	void BallMovement();

	void Reset();

	GameObject* m_pBall;
	GameObject* m_pPlayerOne;
	GameObject* m_pPlayerTwo;
	GameObject* m_pTopWall;
	GameObject* m_pBottomWall;
	GameObject* m_pRightWall;
	GameObject* m_pLeftWall;

	float m_BallSpeed = 5.f;
	float m_PlayerSpeed = 0.5f;
	float m_BallDirX = 0.f;
	float m_BallDirZ = 0.f;

	bool m_InvertZ = false;
	bool m_InvertX = false;
	bool m_CanReset = false;
};
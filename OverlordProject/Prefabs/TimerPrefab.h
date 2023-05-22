#pragma once
class TimerPrefab final : public GameObject
{
public:
	TimerPrefab(int totalTime, XMFLOAT2 timerPosition);
	~TimerPrefab() override = default;

	TimerPrefab(const TimerPrefab& other) = delete;
	TimerPrefab(TimerPrefab&& other) noexcept = delete;
	TimerPrefab& operator=(const TimerPrefab& other) = delete;
	TimerPrefab& operator=(TimerPrefab&& other) noexcept = delete;

	void ShowTime();
	bool IsTimeOut();
	void Reset();

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	int m_TotalTime;
	float m_CurrentTime{ 0 };
	SpriteFont* m_pFont{};
	XMFLOAT2 m_Position{};
};
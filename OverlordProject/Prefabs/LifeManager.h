#pragma once
class LifeManager final : public GameObject
{
public:
	LifeManager(int maxLife, float width = 1.f, float height = 1.f, float depth = 1.f, float x = 0.f, float y = 0.f, float z = 0.f);
	LifeManager(int maxLife, const XMFLOAT3& dimensions, const XMFLOAT3& positon);
	~LifeManager() override = default;

	LifeManager(const LifeManager& other) = delete;
	LifeManager(LifeManager&& other) noexcept = delete;
	LifeManager& operator=(const LifeManager& other) = delete;
	LifeManager& operator=(LifeManager&& other) noexcept = delete;

	void RemoveLife();
	void AddLife();
	bool IsDead();
	void Reset();


protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	void DrawAllLives();

	int m_MaxLife;
	int m_CurrentLife;
	XMFLOAT3 m_Position{};
	XMFLOAT3 m_Dimensions{};
	std::vector<GameObject*> m_EmptyHearts;
	std::vector<GameObject*> m_Hearts;
};
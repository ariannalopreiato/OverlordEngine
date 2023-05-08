#pragma once
class Character;
class Ladder final : public GameObject
{
public:
	Ladder(float width = 1.f, float height = 1.f, float depth = 1.f, const XMFLOAT3& position = { 0.f, 0.f, 0.f }, const XMFLOAT3& rotation = { 0.f, 0.f, 0.f });
	Ladder(const XMFLOAT3& dimensions, const XMFLOAT3& positon, const XMFLOAT3& rotation);
	~Ladder() override = default;

	Ladder(const Ladder& other) = delete;
	Ladder(Ladder&& other) noexcept = delete;
	Ladder& operator=(const Ladder& other) = delete;
	Ladder& operator=(Ladder&& other) noexcept = delete;

	void SetPlayer(Character*& player) { m_pPlayer = player; }
	bool IsOnLadder() { return m_IsOnLadder; }

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	float m_Width{}, m_Height{}, m_Depth{};
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	GameObject* m_pLadder;
	Character* m_pPlayer;
	bool m_IsOnLadder = false;
};


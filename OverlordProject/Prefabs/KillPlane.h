#pragma once
class Character;
class KillPlane final : public GameObject
{
public:
	KillPlane(Character* pPlayer, const XMFLOAT3& position = { 0.f, 0.f, 0.f }, const XMFLOAT3& scale = { 1.f, 1.f, 1.f });
	~KillPlane() override = default;

	KillPlane(const KillPlane& other) = delete;
	KillPlane(KillPlane&& other) noexcept = delete;
	KillPlane& operator=(const KillPlane& other) = delete;
	KillPlane& operator=(KillPlane&& other) noexcept = delete;

	bool GetIsHit() { return m_IsHit; }
	void Reset() { m_IsHit = false; }

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	XMFLOAT3 m_Scale;
	XMFLOAT3 m_Position;
	bool m_IsHit{ false };
	Character* m_pPlayer;
};
#pragma once

class CollectiblePrefab final : public GameObject
{
public:
	CollectiblePrefab(const std::wstring& texture, const std::wstring& model, const XMFLOAT3& position = { 0.f, 0.f, 0.f }, const XMFLOAT3& rotation = { 0.f, 0.f, 0.f }, const XMFLOAT3& scale = {1.f, 1.f, 1.f}, float pivotOffset = 0.f);
	~CollectiblePrefab() override = default;

	CollectiblePrefab(const CollectiblePrefab& other) = delete;
	CollectiblePrefab(CollectiblePrefab&& other) noexcept = delete;
	CollectiblePrefab& operator=(const CollectiblePrefab& other) = delete;
	CollectiblePrefab& operator=(CollectiblePrefab&& other) noexcept = delete;

	bool GetIsCollected() { return m_IsCollected; }

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	XMFLOAT3 m_Scale;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	std::wstring m_Texture;
	std::wstring m_Model;
	GameObject* m_ModelMesh;
	float m_PivotOffset;
	float m_Rotate;
	bool m_IsCollected{ false };
};
#pragma once
class CollectiblePrefab final : public GameObject
{
public:
	CollectiblePrefab(const std::wstring& model, int value = 1, const XMFLOAT3& position = { 0.f, 0.f, 0.f }, const XMFLOAT3& rotation = { 0.f, 0.f, 0.f }, const XMFLOAT3& scale = {1.f, 1.f, 1.f}, float pivotOffset = 0.f);
	~CollectiblePrefab() override = default;

	CollectiblePrefab(const CollectiblePrefab& other) = delete;
	CollectiblePrefab(CollectiblePrefab&& other) noexcept = delete;
	CollectiblePrefab& operator=(const CollectiblePrefab& other) = delete;
	CollectiblePrefab& operator=(CollectiblePrefab&& other) noexcept = delete;

	bool GetIsCollected() { return m_IsCollected; }
	int GetValue() { return m_Value; }
	ModelComponent* GetModel() { return m_pModelMesh->GetComponent<ModelComponent>(); }

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	XMFLOAT3 m_Scale;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	std::wstring m_Texture;
	std::wstring m_Model;
	GameObject* m_pModelMesh;
	float m_PivotOffset;
	float m_Rotate{ 0.f };
	bool m_IsCollected{ false };
	int m_Value{ 0 };
	bool m_CanPlay{ false };
	FMOD::Channel* m_pCollectSound{};
};
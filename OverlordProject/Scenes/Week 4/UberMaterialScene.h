#pragma once
class UberMaterial;
class UberMaterialScene : public GameScene
{
public:
	UberMaterialScene() : GameScene(L"Uber Material Scene") {}
	~UberMaterialScene() override = default;

	UberMaterialScene(const UberMaterialScene& other) = delete;
	UberMaterialScene(UberMaterialScene&& other) noexcept = delete;
	UberMaterialScene& operator=(const UberMaterialScene& other) = delete;
	UberMaterialScene& operator=(UberMaterialScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	UberMaterial* m_pUberMaterial;
	GameObject* m_pSphere{ nullptr };
	float m_Rotation{};
};


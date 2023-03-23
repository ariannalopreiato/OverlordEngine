#pragma once

class ModelTestScene : public GameScene
{
public:
	ModelTestScene() : GameScene(L"Model Test Scene") {}
	~ModelTestScene() override = default;

	ModelTestScene(const ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	//void Update() override;
	//void Draw() override;
	//void OnGUI() override;

private:
};


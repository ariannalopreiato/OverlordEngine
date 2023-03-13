#pragma once
class ComponentTestScene : public GameScene
{
public:
	ComponentTestScene() : GameScene(L"Component Test Scene") {}
	~ComponentTestScene() override = default;

	ComponentTestScene(const ComponentTestScene& other) = delete;
	ComponentTestScene(ComponentTestScene&& other) noexcept = delete;
	ComponentTestScene& operator=(const ComponentTestScene& other) = delete;
	ComponentTestScene& operator=(ComponentTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	//void Update() override;
	//void Draw() override;
	//void OnGUI() override;

private:

};


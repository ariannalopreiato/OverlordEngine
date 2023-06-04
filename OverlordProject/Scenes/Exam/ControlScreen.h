#pragma once
class ControlScreen : public GameScene
{
public:
	ControlScreen() :GameScene(L"Control Screen") {}
	~ControlScreen() override = default;

	ControlScreen(const ControlScreen& other) = delete;
	ControlScreen(ControlScreen&& other) noexcept = delete;
	ControlScreen& operator=(const ControlScreen& other) = delete;
	ControlScreen& operator=(ControlScreen&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	GameObject* m_pBackground{};
	GameObject* m_Continue{};
};
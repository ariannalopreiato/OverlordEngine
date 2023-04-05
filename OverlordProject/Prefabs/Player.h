#pragma once
class Player final : public GameObject
{
public:
	Player(const std::wstring& texture, const std::wstring& model);
	~Player();

	Player(const Player& other) = delete;
	Player(Player&& other) noexcept = delete;
	Player& operator=(const Player& other) = delete;
	Player& operator=(Player&& other) noexcept = delete;

	void Initialize();
	void Update();

private:
	std::wstring m_Texture;
	std::wstring m_Model;

	ModelAnimator* pAnimator{};

	int m_AnimationClipId{ 0 };
	float m_AnimationSpeed{ 1.f };

	char** m_ClipNames{};
	UINT m_ClipCount{};
};


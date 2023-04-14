#pragma once

class BoneObject;
class SoftwareSkinningScene_1 : public GameScene
{
public:
	SoftwareSkinningScene_1() : GameScene(L"Software Skinning Scene 1") {}
	~SoftwareSkinningScene_1() override = default;

	SoftwareSkinningScene_1(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{};
	BoneObject* m_pBone1{};
	float m_BoneRotation{};
	int m_RotationSign{ 1 };
	float m_RotationAngle{ 45.f };

	float m_Bone0Rotation{};
	float m_Bone1Rotation{};
	bool m_AutoRotate{ true };
};
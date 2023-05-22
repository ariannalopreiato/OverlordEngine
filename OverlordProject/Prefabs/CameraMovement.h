#pragma once

class Character;
class Camera;
class CameraMovement : public BaseComponent
{
public:
	CameraMovement(GameObject* player, XMFLOAT3 cameraOffset);
	~CameraMovement() = default;

	CameraMovement(const CameraMovement& other) = delete;
	CameraMovement(CameraMovement&& other) noexcept = delete;
	CameraMovement& operator=(const CameraMovement& other) = delete;
	CameraMovement& operator=(CameraMovement&& other) noexcept = delete;

	void CameraRotation(const SceneContext& sceneContext);
	void KeepPlayerCentered();
	void GetCloserToPlayer(CollisionGroup ignoreGroups = CollisionGroup::None);
	void FollowPlayer();

	void SetCameraMovement(int moveLeft, int moveRight);


protected:
	void Initialize(const SceneContext& /*sceneContext*/);
	void Update(const SceneContext& /*sceneContext*/);
	void LateUpdate(const SceneContext& /*sceneContext*/);

private:
	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };

	//float m_CurrentDistance;
	//float m_MaxDistance;
	//float m_MinDistance;

	XMFLOAT3 m_CameraOffset;

	GameObject* m_pPlayer{ nullptr };

	float m_RotatingSpeed{ 5.f };
	float m_RotationDirection{ 0.f };

	float m_CameraHeight;
	float m_CameraDistance;
};
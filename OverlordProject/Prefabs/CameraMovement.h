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

	void CameraRotation();
	void KeepPlayerCentered();
	void GetCloserToPlayer();
	void FollowPlayer();

protected:
	void Initialize(const SceneContext& /*sceneContext*/);
	void Update(const SceneContext& /*sceneContext*/);
	void LateUpdate(const SceneContext& /*sceneContext*/);

private:

	float m_CurrentDistance;
	float m_MaxDistance;
	float m_MinDistance;

	XMFLOAT3 m_CameraOffset;

	GameObject* m_pPlayer{ nullptr };

};
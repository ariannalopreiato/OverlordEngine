#pragma once

class Character;
class Camera;
class CameraMovement : public BaseComponent
{
public:
	CameraMovement(GameObject* player);
	~CameraMovement() = default;

	CameraMovement(const CameraMovement& other) = delete;
	CameraMovement(CameraMovement&& other) noexcept = delete;
	CameraMovement& operator=(const CameraMovement& other) = delete;
	CameraMovement& operator=(CameraMovement&& other) noexcept = delete;

	void CameraRotation();
	void FollowPlayer();
	void GetCloserToPlayer();

protected:
	void Initialize(const SceneContext& /*sceneContext*/);
	void Update(const SceneContext& /*sceneContext*/);
	void LateUpdate(const SceneContext& /*sceneContext*/);

private:
	XMVECTOR m_VectorToPlayer{ };
	XMVECTOR m_WorldForward{ };

	float m_CurrentDistance;
	float m_MaxDistance;
	float m_MinDistance;

	GameObject* m_pPlayer{ nullptr };

};
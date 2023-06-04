#pragma once
struct CharacterDesc
{
	CharacterDesc(
		PxMaterial* pMaterial,
		float radius = .5f,
		float height = 2.f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
	}

	float maxMoveSpeed{ 15.f };
	float maxFallSpeed{ 15.f };

	float JumpSpeed{ 15.f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ .3f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 60.f };

	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Jump{ -1 };
};

class CameraMovement;
class Character : public GameObject
{
public:
	Character(const CharacterDesc& characterDesc);
	~Character() override = default;

	Character(const Character& other) = delete;
	Character(Character&& other) noexcept = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) noexcept = delete;

	void ScalePlayerMesh(float scale);
	void DrawImGui();
	void SetIsClimbing(bool isClimbing) { m_CanClimb = isClimbing; }
	bool GetIsClimbing() { return m_CanClimb; }
	XMFLOAT3 GetVelocity() const { return m_TotalVelocity; }

	void SetActiveCamera(CameraComponent* camera) { m_pCameraComponent = camera; }
	void SetCameraMovement(CameraMovement* camera) { m_pCameraMovement = camera; }

	enum AnimationState
	{
		idle,
		run,
		jump
	};

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	
private:
	void SetAnimation(AnimationState state);
	void PlayRandomJumpSound();

	CameraComponent* m_pCameraComponent{};
	ControllerComponent* m_pControllerComponent{};
	CameraMovement* m_pCameraMovement{};

	bool m_CanClimb = false;

	ModelAnimator* m_pAnimator{};

	AnimationState m_CurrentState;
	AnimationState m_PreviousState;

	ParticleEmitterComponent* m_pEmitter{};

	char** m_ClipNames{};
	UINT m_ClipCount{};

	CharacterDesc m_CharacterDesc;
	float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{};								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)

	XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)

	FMOD::Channel* m_Hop{};
	FMOD::Channel* m_Hup{};
	FMOD::Channel* m_Yup{};
	FMOD::Channel* m_CurrentJumpSound{};
};


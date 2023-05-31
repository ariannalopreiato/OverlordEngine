#include "stdafx.h"
#include "Character.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "CameraMovement.h"

Character::Character(const CharacterDesc& characterDesc, const std::wstring& texture, const std::wstring& model, float pivotOffset, bool isAnimated) :
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime),
	m_Texture{ texture },
	m_Model{ model },
	m_IsAnimated{ isAnimated },
	m_PivotOffset{ pivotOffset }
{}

void Character::Initialize(const SceneContext& /*sceneContext*/)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	//Camera
	float moveBack = -13.f;
	const auto pCamera = AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera
	pCamera->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * .5f, moveBack);

	if (!m_Texture.empty() && !m_Model.empty())
	{
		const auto pSkinnedMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
		pSkinnedMaterial->SetDiffuseTexture(m_Texture);

		m_ModelMesh = AddChild(new GameObject);
		auto pModel = m_ModelMesh->AddComponent(new ModelComponent(m_Model));
		pModel->GetTransform()->Translate(0.f, -m_PivotOffset, 0.f);
		pModel->SetMaterial(pSkinnedMaterial);	

		m_pAnimator = pModel->GetAnimator();
	}
}

void Character::ScalePlayerMesh(float scale)
{
	if(m_ModelMesh)
		m_ModelMesh->GetTransform()->Scale(scale);
}


void Character::Update(const SceneContext& sceneContext)
{
	constexpr float epsilon{ 0.001f }; //Constant that can be used to compare if a float is near zero

	//***************
	//HANDLE INPUT

	//## Input Gathering (move)
	XMFLOAT3 move = { 0.f, 0.f, 0.f }; //Uncomment

	//if it's inside the ladder trigger
	if (m_CanClimb)
	{
		//if it's still on the ground
		if (m_IsGrounded)
		{
			//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
			//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement
			if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
				move.z = -1;

			if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
			{
				move.z = 1;
				m_IsGrounded = false;
			}
		}
		else
		{
			//if it's not grounded and it can climb it means it's already climbing -> move vertically
			if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
				move.y = 1;
			if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
				move.y = -1;
		}
	}
	else
	{
		//move.x should contain a 1 (Right) or -1 (Left) based on the active input (check corresponding actionId in m_CharacterDesc)
		//Optional: if move.x is near zero (abs(move.x) < epsilon), you could use the Left ThumbStickPosition.x for movement
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft))
			move.x = -1;
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
			move.x = 1;
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
			move.z = -1;
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
			move.z = 1;
	}

	//************************
	//GATHERING TRANSFORM INFO

	//Retrieve the forward & right vector (as XMVECTOR) from the current camera
	auto forward = XMLoadFloat3(&m_pCameraMovement->GetTransform()->GetForward());
	auto right = XMLoadFloat3(&m_pCameraMovement->GetTransform()->GetRight());

	//********
	//MOVEMENT

	//## Horizontal Velocity (Forward/Backward/Right/Left)
	//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
	auto currentAcceleration = m_MoveAcceleration * sceneContext.pGameTime->GetElapsed();

	//If the character is moving (= input is pressed)
	if (abs(move.x) > epsilon || abs(move.z) > epsilon || abs(move.y) > epsilon)
	{
		//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input
		XMStoreFloat3(&m_CurrentDirection, XMVector3Normalize(move.x * right + move.z * forward));

		//Increase the current MoveSpeed with the current Acceleration (m_MoveSpeed)
		m_MoveSpeed += currentAcceleration;

		//Make sure the current MoveSpeed stays below the maximum MoveSpeed (CharacterDesc::maxMoveSpeed)
		if (m_MoveSpeed > m_CharacterDesc.maxMoveSpeed)
			m_MoveSpeed = m_CharacterDesc.maxMoveSpeed;
	}
	//Else (character is not moving, or stopped moving)
	else
	{
		//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
		m_MoveSpeed -= currentAcceleration;

		//Make sure the current MoveSpeed doesn't get smaller than zero
		if (m_MoveSpeed < 0.0f)
			m_MoveSpeed = 0.f;
	}

	//Now we can calculate the Horizontal Velocity which should be stored in m_TotalVelocity.xz
	//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)
	//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
	//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)
	m_TotalVelocity.x = m_CurrentDirection.x * m_MoveSpeed;
	m_TotalVelocity.z = m_CurrentDirection.z * m_MoveSpeed;

	//********
	//ROTATION
	float targetAngle = XMConvertToDegrees(atan2(m_TotalVelocity.x, m_TotalVelocity.z));

	GetTransform()->Rotate(0, targetAngle - 180, 0);

	//## Vertical Movement (Jump/Fall)
	//If the Controller Component is NOT grounded (= freefall)	
	if (!m_pControllerComponent->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN))
	{	//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
		//Make sure that the minimum speed stays above -CharacterDesc::maxFallSpeed (negative!)
		m_TotalVelocity.y -= m_FallAcceleration * sceneContext.pGameTime->GetElapsed();
		if (m_TotalVelocity.y < -m_CharacterDesc.maxFallSpeed)
			m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;
	}
	//Else If the jump action is triggered
	else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump))
	{
		//Set m_TotalVelocity.y equal to CharacterDesc::JumpSpeed
		m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
	}
	//Else (=Character is grounded, no input pressed)
	else
	{
		//m_TotalVelocity.y is zero
		m_TotalVelocity.y = -0.01f;
		m_IsGrounded = true;
	}

	//************
	//DISPLACEMENT

	//The displacement required to move the Character Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
	//Calculate the displacement (m) for the current frame and move the ControllerComponent
	XMFLOAT3 displacement;
	displacement.x = m_TotalVelocity.x * sceneContext.pGameTime->GetElapsed();
	displacement.y = m_TotalVelocity.y * sceneContext.pGameTime->GetElapsed();
	displacement.z = m_TotalVelocity.z * sceneContext.pGameTime->GetElapsed();
	m_pControllerComponent->Move(displacement);
	//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
	//Also, it can be usefull to use a seperate RayCast to check if the character is grounded (more responsive)
}


void Character::DrawImGui()
{
	if (ImGui::CollapsingHeader("Character"))
	{
		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if (ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}
}
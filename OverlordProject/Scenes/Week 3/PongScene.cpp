#include "stdafx.h"
#include "PongScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"

void PongScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = XMFLOAT4{ Colors::Black };

	InitializeBall();

	float distance{ 35.f };
	InitializePlayer(-distance, 0.f, 0.f, m_pPlayerOne);
	InitializePlayer(distance, 0.f, 0.f, m_pPlayerTwo);

	InitializeTriggers();

	const auto fixedCamera = new FixedCamera();
	fixedCamera->GetTransform()->Translate(0.f, 60.f, 0.f);
	fixedCamera->GetTransform()->Rotate(90.f, 0.f, 0.f);
	AddChild(fixedCamera);
	SetActiveCamera(fixedCamera->GetComponent<CameraComponent>());
}

void PongScene::Update()
{
	PlayerOneMovement();
	PlayerTwoMovement();

	if (InputManager::IsKeyboardKey(InputState::released, VK_SPACE))
	{
		if (m_BallDirX == 0.f && m_BallDirZ == 0.f)
		{
			BallMovement();
		}
	}

	if (m_CanReset)
		Reset();

	if (m_InvertX)
	{
		m_BallDirX *= -1;
		m_InvertX = false;
	}

	if (m_InvertZ)
	{
		m_BallDirZ *= -1;
		m_InvertZ = false;
	}

	PxVec3 velocity = PxVec3{ m_BallDirX * m_BallSpeed, 0.f, m_BallDirZ * m_BallSpeed };
	auto ballPos = m_pBall->GetTransform()->GetPosition();
	m_pBall->GetComponent<RigidBodyComponent>()->GetPxRigidActor()->is<PxRigidDynamic>()->setLinearVelocity(velocity);
}

void PongScene::Draw()
{
	//Optional
}

void PongScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}

void PongScene::InitializeBall()
{
	//MATERIAL
	auto& physx = PxGetPhysics();
	auto pBouncyMaterial = physx.createMaterial(0.f, 0.f, 1.f);

	//RED SPHERE
	float radius{ 1.f };
	m_pBall = new SpherePrefab(radius, 10, XMFLOAT4{ Colors::Red });
	AddChild(m_pBall);

	m_pBall->GetTransform()->Translate(0.f, 0.f, 0.f);

	auto pRigidBody = m_pBall->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxSphereGeometry{ radius }, *pBouncyMaterial, true);
	pRigidBody->SetConstraint(RigidBodyConstraint::TransY, false);
	m_pBall->SetOnTriggerCallBack([=](GameObject*, GameObject* otherObjectPtr, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
				if (otherObjectPtr == m_pTopWall || otherObjectPtr == m_pBottomWall)
					m_InvertZ = true;

				if (otherObjectPtr == m_pPlayerOne || otherObjectPtr == m_pPlayerTwo)
					m_InvertX = true;

				if (otherObjectPtr == m_pLeftWall || otherObjectPtr == m_pRightWall)
					m_CanReset = true;
			}
		});

	m_BallDirX = 0.f;
	m_BallDirZ = 0.f;
}

void PongScene::InitializePlayer(float x, float y, float z, GameObject*& player)
{
	//MATERIAL
	auto& physx = PxGetPhysics();
	auto pBouncyMaterial = physx.createMaterial(0.5f, 0.5f, 1.f);

	//PLAYER
	float height{ 8.f };
	float width{ 2.f };
	player = new CubePrefab(width, width, height, XMFLOAT4{ Colors::White });
	AddChild(player);

	player->GetTransform()->Translate(x, y, z);

	auto pRigidBody = player->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ width / 2, width / 2, height / 2 }, *pBouncyMaterial);
	pRigidBody->SetConstraint(RigidBodyConstraint::TransX | RigidBodyConstraint::TransZ, false);
}

void PongScene::InitializeTriggers()
{
	float heightHor{ 2.f };
	float widthHor{ m_SceneContext.windowWidth };

	float heightVer{ m_SceneContext.windowHeight };
	float widthVer{ 2.f };

	float zMovement{ 25.f };
	float xMovement{ 45.f };

	auto& physx = PxGetPhysics();
	auto pMaterial = physx.createMaterial(0.5f, 0.5f, 0.5f);

	//TOP WALL
	m_pTopWall = new CubePrefab(widthHor, heightHor, heightHor, XMFLOAT4{ Colors::Transparent });
	AddChild(m_pTopWall);
	m_pTopWall->GetTransform()->Translate(XMFLOAT3{ 0.f, 0.f, zMovement });
	auto pRigidBody = m_pTopWall->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ widthHor/2, heightHor/2, heightHor/2 }, *pMaterial);

	//BOTTOM WALL
	m_pBottomWall = new CubePrefab(widthHor, heightHor, heightHor, XMFLOAT4{ Colors::Transparent });
	AddChild(m_pBottomWall);
	m_pBottomWall->GetTransform()->Translate(XMFLOAT3{ 0.f, 0.f, -zMovement });
	pRigidBody = m_pBottomWall->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ widthHor / 2, heightHor / 2, heightHor / 2 }, *pMaterial);

	//LEFT WALL
	m_pLeftWall = new CubePrefab(widthVer, widthVer, heightVer, XMFLOAT4{ Colors::Transparent });
	AddChild(m_pLeftWall);
	m_pLeftWall->GetTransform()->Translate(XMFLOAT3{ xMovement, 0.f, 0.f });
	pRigidBody = m_pLeftWall->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ widthVer / 2, widthVer / 2, heightVer / 2 }, *pMaterial);

	//RIGHT WALL
	m_pRightWall = new CubePrefab(widthVer, widthVer, heightVer, XMFLOAT4{ Colors::Transparent });
	AddChild(m_pRightWall);
	m_pRightWall->GetTransform()->Translate(XMFLOAT3{ -xMovement, 0.f, 0.f });
	pRigidBody = m_pRightWall->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ widthVer / 2, widthVer / 2, heightVer / 2 }, *pMaterial);
}

void PongScene::PlayerOneMovement()
{
	auto windowHeight = m_SceneContext.windowHeight;

	if (InputManager::IsKeyboardKey(InputState::down, 'W'))
	{
		auto currentPos = m_pPlayerOne->GetTransform()->GetPosition();
		if (currentPos.y + m_PlayerSpeed <= windowHeight / 2)
			m_pPlayerOne->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z + m_PlayerSpeed);
	}

	if (InputManager::IsKeyboardKey(InputState::down, 'S'))
	{
		auto currentPos = m_pPlayerOne->GetTransform()->GetPosition();
		if (currentPos.y - m_PlayerSpeed >= -windowHeight / 2)
			m_pPlayerOne->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z - m_PlayerSpeed);
	}
}

void PongScene::PlayerTwoMovement()
{
	auto windowHeight = m_SceneContext.windowHeight;

	if (InputManager::IsKeyboardKey(InputState::down, VK_UP))
	{
		auto currentPos = m_pPlayerTwo->GetTransform()->GetPosition();
		if(currentPos.y + m_PlayerSpeed <= windowHeight / 2)
			m_pPlayerTwo->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z + m_PlayerSpeed);
	}

	if (InputManager::IsKeyboardKey(InputState::down, VK_DOWN))
	{
		auto currentPos = m_pPlayerTwo->GetTransform()->GetPosition();
		if (currentPos.y - m_PlayerSpeed >= -windowHeight / 2)
		m_pPlayerTwo->GetTransform()->Translate(currentPos.x, currentPos.y, currentPos.z - m_PlayerSpeed);
	}
}

void PongScene::BallMovement()
{
	float randomX = float(rand() % 2);
	float randomZ = float(rand() % 2);

	if (randomX > 0)
		randomX *= m_BallSpeed;
	else
		randomX = -m_BallSpeed;

	if (randomZ > 0)
		randomZ *= m_BallSpeed;
	else
		randomZ = -m_BallSpeed;

	m_BallDirX = randomX;
	m_BallDirZ = randomZ;
}

void PongScene::Reset()
{
	RemoveChild(m_pBall, true);
	RemoveChild(m_pPlayerOne, true);
	RemoveChild(m_pPlayerTwo, true);

	InitializeBall();
	float distance{ 35.f };
	InitializePlayer(-distance, 0.f, 0.f, m_pPlayerOne);
	InitializePlayer(distance, 0.f, 0.f, m_pPlayerTwo);

	m_CanReset = false;
}

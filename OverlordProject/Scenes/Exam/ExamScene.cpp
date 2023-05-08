#include "stdafx.h"
#include "ExamScene.h"
#include "Prefabs/Character.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/CollectiblePrefab.h"
#include "Prefabs/Ladder.h"

void ExamScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;

	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//Character
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;

	float pivotOffset = 1.3f;
	m_pPlayer = AddChild(new Character(characterDesc, L"Textures/body.png", L"Meshes/link.ovm", pivotOffset));
	m_pPlayer->ScalePlayerMesh(0.015f);
	InitializePlayer();

	//Level
	const auto pLevelMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();

	const auto pLevelObject = AddChild(new GameObject());
	const auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Level/windfall.ovm"));
	pLevelMesh->SetMaterial(pLevelMaterial);

	const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Level/windfall.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ .015f, .015f, .015f })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale(.015f, .015f, .015f);

	//Ladders
	PositionLaddersTrigger();

	//Collectibles
	InitializeCollectibles();

	//Input
	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);	

	//BACKGROUND MUSIC


	//SKYBOX
}

void ExamScene::Update()
{
	//std::cout << "POSITION" << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().x << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().y << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().z << std::endl;
}

void ExamScene::Draw()
{
}

void ExamScene::OnGUI()
{

}

void ExamScene::InitializePlayer()
{
	//reposition player
	m_pPlayer->GetTransform()->Translate(15.f, 3.f, -49.f);
	//m_pPlayer->GetTransform()->Translate(8.9f, 17.f, 50.f); big ladder
}

void ExamScene::InitializeCollectibles()
{
	if (!m_Collectibles.empty())
	{
		for (auto& collectible : m_Collectibles)
		{
			SafeDelete(collectible);
		}
		m_Collectibles.clear();
	}

	m_Collectibles.emplace_back(AddChild(new CollectiblePrefab(L"Textures/rupee.png", L"Meshes/rupee.ovm", { 1.37f, 1.12f, -47.6f }, { 90.f, 0.f, 0.f })));
}

void ExamScene::PositionLaddersTrigger()
{
	XMFLOAT3 ladderDimensions{ 0.8f, 5.9f, 0.5f };
	m_Ladders.emplace_back(AddChild(new Ladder(ladderDimensions, { -2.f, 18.7f, 9.f }, {0.f, 30.f, 0.f})));
	m_Ladders.emplace_back(AddChild(new Ladder(ladderDimensions, { 17.4f, 18.7f, 10.6f }, { 0.f, -35.f, 0.f })));
	XMFLOAT3 bigLadderDimensions{ 0.8f, 20.f, 0.5f };
	m_Ladders.emplace_back(AddChild(new Ladder(bigLadderDimensions, { 10.7f, 40.7f, 47.1f }, { 0.f, -35.f, 0.f })));

	for (const auto& ladder : m_Ladders)
	{
		ladder->SetPlayer(m_pPlayer);
	}
}

void ExamScene::Reset()
{
	InitializePlayer();
	InitializeCollectibles();
}
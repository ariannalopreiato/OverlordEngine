#include "stdafx.h"
#include "ExamScene.h"
#include "Prefabs/Character.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/CollectiblePrefab.h"

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

	m_Collectibles.emplace_back(AddChild(new CollectiblePrefab(L"Textures/rupee.png", L"Meshes/rupee.ovm", { 15.f, 2.f, -49.f }, { 90.f, 0.f, 0.f })));
}

void ExamScene::Reset()
{
	InitializePlayer();
	InitializeCollectibles();
}
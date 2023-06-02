#include "stdafx.h"
#include "ExamScene.h"
#include "Prefabs/Character.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/SkyboxMaterial.h"
#include "Prefabs/CollectiblePrefab.h"
#include "Prefabs/Ladder.h"
#include "Prefabs/CameraMovement.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Post/PostBloom.h"
#include "Prefabs/TimerPrefab.h"
#include "Prefabs/LifeManager.h"

void ExamScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false; 
	m_SceneContext.settings.drawPhysXDebug = false;

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Sheerwood_32.fnt");

	//GROUND PLANE
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//SKYBOX
	auto pSkyBox = AddChild(new GameObject());
	const auto pModel = pSkyBox->AddComponent(new ModelComponent(L"Meshes/skybox.ovm"));

	const auto pMaterial = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/skybox.png");
	pModel->SetMaterial(pMaterial);
	pSkyBox->GetTransform()->Scale(400, 350, 400);
	pSkyBox->GetTransform()->Rotate(0, 135, 0);

	//CHARACTER
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;

	float pivotOffset = 1.3f;
	m_pPlayer = AddChild(new Character(characterDesc, L"Textures/body.png", L"Meshes/Player/link.ovm", pivotOffset, true));
	m_pPlayer->ScalePlayerMesh(0.015f);
	InitializePlayer();

	//CAMERA
	const auto cameraObj = AddChild(new GameObject());
	auto cameraMovement = cameraObj->AddComponent(new CameraMovement(m_pPlayer, {8.f, 4.f, 6.f}));
	auto cameraComponent = cameraObj->AddComponent(new CameraComponent());
	SetActiveCamera(cameraComponent);

	cameraObj->GetTransform()->Translate(10.f, 3.f, -49.f);

	m_pPlayer->SetCameraMovement(cameraMovement);

	cameraMovement->SetCameraMovement(MoveCameraLeft, MoveCameraRight);

	//Ladders
	//PositionLaddersTrigger();

	//COLLECTIBLES
	InitializeCollectibles();

	//INPUT
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

	inputAction = InputAction(MoveCameraLeft, InputState::down, VK_LEFT);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(MoveCameraRight, InputState::down, VK_RIGHT);
	m_SceneContext.pInput->AddInputAction(inputAction);

	//TIMER
	m_pTimer = AddChild((new TimerPrefab(241, XMFLOAT2{ m_SceneContext.windowWidth/2 - 50.f, 10.f})));

	//BACKGROUND MUSIC


	//POST PROCESSING - BLOOM
	auto bloom = MaterialManager::Get()->CreateMaterial<PostBloom>();
	AddPostProcessingEffect(bloom);

	//LIFE
	m_pLifeManager = AddChild(new LifeManager(3, {0.13f, 0.13f, 0.13f}, {20.f, 80.f, 0.f}));

	//LOAD LEVEL
	LoadLevel();
}

void ExamScene::OnGUI()
{
	ImGui::Checkbox("Draw ShadowMap", &m_DrawShadowMap);
	ImGui::SliderFloat("ShadowMap Scale", &m_ShadowMapScale, 0.f, 1.f);
}

void ExamScene::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(0))
	{
		const auto pCameraTransform = m_SceneContext.pCamera->GetTransform();
		m_SceneContext.pLights->SetDirectionalLight(pCameraTransform->GetPosition(), pCameraTransform->GetForward());
	}

	if (m_DrawShadowMap) {

		ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { m_ShadowMapScale, m_ShadowMapScale }, { 1.f,0.f });
	}

	//std::cout << "POSITION" << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().x << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().y << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().z << std::endl;

	//if there is still time -> update the points
	if (!m_pTimer->IsTimeOut())
	{
		CheckForCollectibles();
		DisplayPoints();

		if (InputManager::IsKeyboardKey(InputState::released, VK_DELETE))
		{
			SceneManager::Get()->SetActiveGameScene(L"Pause Menu");
		}

		//check if all the rupees have been collected
		if (CheckGameWon())
		{
			Reset();
			SceneManager::Get()->SetActiveGameScene(L"Win Screen");
		}
	}
	else //if the time runs out
	{
		//check if all the rupees have been collected
		if (CheckGameWon())
		{
			Reset();
			SceneManager::Get()->SetActiveGameScene(L"Win Screen");
		}
		else
		{
			Reset();
			SceneManager::Get()->SetActiveGameScene(L"Game Over");
		}
	}	

	if (m_pLifeManager->IsDead())
	{
		Reset();
		SceneManager::Get()->SetActiveGameScene(L"Game Over");
	}
}

void ExamScene::LateUpdate()
{}

void ExamScene::Draw()
{
}

void ExamScene::IncreasePoints()
{
	++m_CurrentPoints;
}

bool ExamScene::CheckGameWon()
{
	if (m_CurrentPoints == m_TotalPoints)
		return true;

	return false;
}

void ExamScene::InitializePlayer()
{
	//reposition player
	m_pPlayer->GetTransform()->Translate(15.f, 3.f, -49.f);
	//m_pPlayer->GetTransform()->Translate(8.9f, 17.f, 50.f); big ladder
}

void ExamScene::InitializeCollectibles()
{
	if (!m_pCollectibles.empty())
	{
		for (auto& collectible : m_pCollectibles)
			RemoveChild(collectible);

		m_pCollectibles.clear();
	}

	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, { 1.37f, 1.12f, -47.6f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {-72.f, 11.f, -29.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {-45.f, 18.f, 56.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {46.f, 8.f, 16.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {57.f, 2.f, 32.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {9.f, 11.f, 73.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {-8.f, 11.f, 63.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {10.f, 17.f, 75.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {12.f, 17.f, 48.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {29.f, 13.f, 22.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {-18.f, 17.f, 13.f }, { 90.f, 0.f, 0.f })));
	m_pCollectibles.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/rupee.ovm", 1, {10.f, 24.f, 3.f }, { 90.f, 0.f, 0.f })));

	//POINTS
	m_TotalPoints = int(m_pCollectibles.size());

	for (const auto& collectible : m_pCollectibles)
	{
		const auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
		auto model = collectible->GetModel();

		pMaterial->SetDiffuseTexture(L"Textures/rupee.png");
		model->SetMaterial(pMaterial);
	}


	if (!m_pHearts.empty())
	{
		for (auto& collectible : m_pHearts)
			RemoveChild(collectible);

		m_pHearts.clear();
	}

	m_pHearts.emplace_back(AddChild(new CollectiblePrefab(L"Meshes/Collectibles/heart.ovm", 1, { 4.37f, 1.12f, -47.6f }, { 0.f, 0.f, 0.f }, {1.2f, 1.2f, 1.2f})));

	for (const auto& heart : m_pHearts)
	{
		const auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
		auto model = heart->GetModel();

		//pMaterial->SetDiffuseTexture(L"Textures/Heart/mat_gd_heart_ut2.png");
		//model->SetMaterial(pMaterial, 0);

		pMaterial->SetDiffuseTexture(L"Textures/Heart/mat_gd_heart_utu.png");
		model->SetMaterial(pMaterial, 1);
	}
}

//void ExamScene::PositionLaddersTrigger()
//{
//	XMFLOAT3 ladderDimensions{ 0.8f, 5.9f, 0.5f };
//	m_Ladders.emplace_back(AddChild(new Ladder(ladderDimensions, { -2.f, 18.7f, 9.f }, {0.f, 30.f, 0.f})));
//	m_Ladders.emplace_back(AddChild(new Ladder(ladderDimensions, { 17.4f, 18.7f, 10.6f }, { 0.f, -35.f, 0.f })));
//	XMFLOAT3 bigLadderDimensions{ 0.8f, 20.f, 0.5f };
//	m_Ladders.emplace_back(AddChild(new Ladder(bigLadderDimensions, { 10.7f, 40.7f, 47.1f }, { 0.f, -35.f, 0.f })));
//
//	for (const auto& ladder : m_Ladders)
//	{
//		ladder->SetPlayer(m_pPlayer);
//	}
//}

void ExamScene::Reset()
{
	InitializePlayer();
	InitializeCollectibles();
	m_CurrentPoints = 0;
	m_pLifeManager->Reset();
	m_pTimer->Reset();
}

void ExamScene::LoadLevel()
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	const int texAmount = 32;

	std::vector<std::wstring> textures;

	textures.reserve(texAmount);

	for (int i = 0; i < texAmount; ++i)
	{
		textures.emplace_back(L"Textures/Level/" + std::to_wstring(i) + L".png");
	}	

	const auto pLevelObject = AddChild(new GameObject());
	const auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Level/windfall.ovm"));

	const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Level/windfall.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ .015f, .015f, .015f })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale(.015f, .015f, .015f);


	for (UINT8 i = 0; i < textures.size(); ++i)
	{
		auto mat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		mat->SetDiffuseTexture(textures[int(i)]);
		pLevelMesh->SetMaterial(mat, i);
	}
}

void ExamScene::CheckForCollectibles()
{
	for (const auto& obj : m_pCollectibles)
	{
		if (obj->GetIsCollected())
		{
			m_CurrentPoints += obj->GetValue();
			RemoveChild(obj);
			m_pCollectibles.erase(std::remove(m_pCollectibles.begin(), m_pCollectibles.end(), obj));
			//todo collected sound
		}
	}

	for (const auto& obj : m_pHearts)
	{
		if (obj->GetIsCollected())
		{
			m_pLifeManager->AddLife();  
			RemoveChild(obj);
			m_pHearts.erase(std::remove(m_pHearts.begin(), m_pHearts.end(), obj));
			//todo collected heart sound
		}
	}
}

void ExamScene::DisplayPoints()
{	
	//std::string point = "x" + std::to_string(m_CurrentPoints);
	std::string point = std::to_string(m_CurrentPoints) + "/" + std::to_string(m_TotalPoints);
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(point), XMFLOAT2(1180.f, 10.f), XMFLOAT4{Colors::LightGoldenrodYellow});
}
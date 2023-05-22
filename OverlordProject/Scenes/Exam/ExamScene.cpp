#include "stdafx.h"
#include "ExamScene.h"
#include "Prefabs/Character.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/CollectiblePrefab.h"
#include "Prefabs/Ladder.h"
#include "Prefabs/CameraMovement.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Post/PostBloom.h"
#include "Prefabs/TimerPrefab.h"

void ExamScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawGrid = false;

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Sheerwood_32.fnt");

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
	m_pPlayer = AddChild(new Character(characterDesc, L"Textures/body.png", L"Meshes/link.ovm", pivotOffset, true));
	m_pPlayer->ScalePlayerMesh(0.015f);
	InitializePlayer();

	//Camera
	const auto cameraObj = AddChild(new GameObject());
	/*auto cameraMovement = */cameraObj->AddComponent(new CameraMovement(m_pPlayer, {8.f, 4.f, 6.f}));
	auto cameraComponent = cameraObj->AddComponent(new CameraComponent());
	SetActiveCamera(cameraComponent);

	cameraObj->GetTransform()->Translate(10.f, 3.f, -49.f);

	LoadLevel();

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

	//TIMER
	m_Timer = AddChild((new TimerPrefab(120, XMFLOAT2{0.f, 0.f})));
	

	//BACKGROUND MUSIC


	//SKYBOX

	//POST PROCESSING - BLOOM
	auto bloom = MaterialManager::Get()->CreateMaterial<PostBloom>();
	AddPostProcessingEffect(bloom);
}

void ExamScene::OnGUI()
{

}

void ExamScene::Update()
{
	//std::cout << "POSITION" << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().x << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().y << std::endl;
	//std::cout << m_pPlayer->GetTransform()->GetPosition().z << std::endl;

	if (!m_Timer->IsTimeOut())
	{
		CheckForCollectibles();
		DisplayPoints();
	}
}

void ExamScene::LateUpdate()
{
}

void ExamScene::Draw()
{
}

void ExamScene::IncreasePoints()
{
	++m_CurrentPoints;
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
			RemoveChild(collectible);

		m_Collectibles.clear();
	}

	m_Collectibles.emplace_back(AddChild(new CollectiblePrefab(L"Textures/rupee.png", L"Meshes/rupee.ovm", 1, { 1.37f, 1.12f, -47.6f }, { 90.f, 0.f, 0.f })));
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

void ExamScene::LoadLevel()
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	const int texAmount = 32;

	std::vector<std::wstring> textures;

	textures.reserve(texAmount);

	textures.emplace_back(L"Textures/Level/m_leaf05.png"); //1

	textures.emplace_back(L"Textures/Level/m_mado02.png");

	textures.emplace_back(L"Textures/Level/wood04.png"); //3

	textures.emplace_back(L"Textures/Level/rotennuno85.png"); //4

	textures.emplace_back(L"Textures/Level/rotennuno4.png"); //5

	textures.emplace_back(L"Textures/Level/m_ren16.png");

	textures.emplace_back(L"Textures/Level/a_kusa.png"); //7

	textures.emplace_back(L"Textures/Level/m_siba04.png");

	textures.emplace_back(L"Textures/Level/m_miti05.png"); //9

	textures.emplace_back(L"Textures/Level/m_komono11.png");

	textures.emplace_back(L"Textures/Level/gakkou03.png"); //11

	textures.emplace_back(L"Textures/Level/m_kabe33.png");

	textures.emplace_back(L"Textures/Level/m_ki13.png");

	textures.emplace_back(L"Textures/Level/a_kusa_yane.png");

	textures.emplace_back(L"Textures/Level/m_siba04.png"); //15

	textures.emplace_back(L"Textures/Level/m_yuka10.png"); //16

	textures.emplace_back(L"Textures/Level/m_ren15.png"); //17

	textures.emplace_back(L"Textures/Level/m_mado08.png");

	textures.emplace_back(L"Textures/Level/a_tuta.png");	

	textures.emplace_back(L"Textures/Level/k_taru05.png");

	textures.emplace_back(L"Textures/Level/gakkou03.png"); 

	textures.emplace_back(L"Textures/Level/wood06.png");

	textures.emplace_back(L"Textures/Level/k_taru04.png"); //23

	textures.emplace_back(L"Textures/Level/m_leaf01.png"); //24

	textures.emplace_back(L"Textures/Level/m_leaf09.png"); //25

	textures.emplace_back(L"Textures/Level/a_iwa.png"); //26

	textures.emplace_back(L"Textures/Level/a_kysakiwa.png");

	textures.emplace_back(L"Textures/Level/rotennuno70.png");

	textures.emplace_back(L"Textures/Level/m_yane08.png"); //29

	textures.emplace_back(L"Textures/Level/m_ki12.png");

	textures.emplace_back(L"Textures/Level/wood01.png"); //31

	textures.emplace_back(L"Textures/Level/m_kabe38.png");



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
	for (const auto& obj : m_Collectibles)
	{
		if (obj->GetIsCollected())
		{
			m_CurrentPoints += obj->GetValue();
			RemoveChild(obj);
			m_Collectibles.erase(std::remove(m_Collectibles.begin(), m_Collectibles.end(), obj));
			//todo collected sound
		}
	}
}

void ExamScene::DisplayPoints()
{	
	std::string point = "x" + std::to_string(m_CurrentPoints);
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(point), XMFLOAT2(50.f, 50.f), XMFLOAT4{Colors::LightGoldenrodYellow});
}
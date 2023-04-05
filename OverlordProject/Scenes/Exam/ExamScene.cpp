#include "stdafx.h"
#include "ExamScene.h"
#include "Prefabs/Player.h"
#include "Materials/ColorMaterial.h"

void ExamScene::Initialize()
{
	//MATERIAL
	auto& physx = PxGetPhysics();
	auto pBouncyMaterial = physx.createMaterial(0.5f, 0.5f, 1.f);

	//GORUNDPLANE
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	//LEVEL
	const auto pLevelMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pLevelMaterial->SetColor(XMFLOAT4{Colors::AliceBlue});
	const auto pObject = AddChild(new GameObject);
	auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/Windfall.ovm"));
	pModel->SetMaterial(pLevelMaterial);

	//PLAYER
	m_pPlayer = new Player(L"Textures/body.png", L"Meshes/link.ovm");
	AddChild(m_pPlayer);
	m_pPlayer->Initialize();

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

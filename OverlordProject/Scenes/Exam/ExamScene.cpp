#include "stdafx.h"
#include "ExamScene.h"
#include "Prefabs/Player.h"

void ExamScene::Initialize()
{
	//MATERIAL
	auto& physx = PxGetPhysics();
	auto pBouncyMaterial = physx.createMaterial(0.5f, 0.5f, 1.f);

	//GORUNDPLANE
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	m_pPlayer = new Player(L"Textures/body.png", L"Meshes/chair.ovm");
	AddChild(m_pPlayer);

	m_pPlayer->Initialize();
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

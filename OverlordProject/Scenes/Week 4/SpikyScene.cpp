#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

void SpikyScene::Initialize()
{
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	m_pSpikyMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();

	m_pSphere = AddChild(new GameObject());
	const auto sphereComponent = m_pSphere->AddComponent(new ModelComponent(L"Meshes/octaSphere.ovm"));
	sphereComponent->SetMaterial(m_pSpikyMaterial);

	m_pSphere->GetTransform()->Scale(15.f);
}

void SpikyScene::Update()
{
	float rotationSpeed = 20.0f; // adjust this to change the speed of rotation
	float deltaTime = m_SceneContext.pGameTime->GetElapsed();
	m_Rotation += rotationSpeed * deltaTime;
	m_pSphere->GetTransform()->Rotate(0.0f, m_Rotation, 0.0f);
}

void SpikyScene::OnGUI()
{
	GameScene::OnGUI();
	m_pSpikyMaterial->DrawImGui();
}
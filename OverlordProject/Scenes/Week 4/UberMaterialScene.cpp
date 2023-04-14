#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/UberMaterial.h"

void UberMaterialScene::Initialize()
{
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	m_pUberMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	m_pUberMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	m_pUberMaterial->SetEnvironmentTexture(L"Textures/Sunol_Cubemap.dds");
	m_pUberMaterial->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	m_pUberMaterial->SetSpecularTexture(L"Textures/Skulls_Heightmap.tga");

	m_pSphere = AddChild(new GameObject());
	const auto sphereComponent = m_pSphere->AddComponent(new ModelComponent(L"Meshes/sphere.ovm"));
	sphereComponent->SetMaterial(m_pUberMaterial);

	m_pSphere->GetTransform()->Scale(20.f);
}

void UberMaterialScene::Update()
{
	float rotationSpeed = 20.0f; // adjust this to change the speed of rotation
	float deltaTime = m_SceneContext.pGameTime->GetElapsed();
	m_Rotation += rotationSpeed * deltaTime;
	m_pSphere->GetTransform()->Rotate(0.0f, m_Rotation, 0.0f);
}

void UberMaterialScene::OnGUI()
{
	GameScene::OnGUI();
	m_pUberMaterial->DrawImGui();
}

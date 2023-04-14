#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/UberMaterial.h"

void UberMaterialScene::Initialize()
{
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	GameObject* pSphere{ nullptr };

	m_pUberMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	m_pUberMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	m_pUberMaterial->SetEnvironmentTexture(L"Textures/Sunol_Cubemap.dds");
	m_pUberMaterial->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	//m_pUberMaterial->SetSpecularTexture(L"Textures/Specular_Level.jpg");
	//m_pUberMaterial->SetOpacityTexture(L"Textures/Skulls_Heightmap.tga");

	pSphere = AddChild(new GameObject());
	const auto sphereComponent = pSphere->AddComponent(new ModelComponent(L"Meshes/sphere.ovm"));
	sphereComponent->SetMaterial(m_pUberMaterial);

	pSphere->GetTransform()->Scale(20.f);
}

void UberMaterialScene::OnGUI()
{
	GameScene::OnGUI();
	m_pUberMaterial->DrawImGui();
}

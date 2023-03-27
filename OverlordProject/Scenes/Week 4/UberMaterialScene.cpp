#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/UberMaterial.h"

void UberMaterialScene::Initialize()
{
	GameObject* pSphere{ nullptr };

	auto pMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	pMaterial->SetEnvironmentTexture(L"Textures/Sunol_Cubemap.dds");
	pMaterial->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	//pMaterial->SetSpecularTexture(L"Textures/Specular_Level.jpg");

	pMaterial->DrawImGui();

	pSphere = AddChild(new GameObject());
	const auto sphereComponent = pSphere->AddComponent(new ModelComponent(L"Meshes/sphere.ovm"));
	sphereComponent->SetMaterial(pMaterial);

	pSphere->GetTransform()->Scale(20.f);
}

void UberMaterialScene::OnGUI()
{
	ImGui::PushFont(nullptr);
	//ImGui::Checkbox("Use normal map", &);
	ImGui::PopFont();
}

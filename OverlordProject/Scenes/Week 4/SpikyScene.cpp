#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

void SpikyScene::Initialize()
{
	

	SpikyMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();
	//pMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");

	m_pSphere = AddChild(new GameObject());
	const auto sphereComponent = m_pSphere->AddComponent(new ModelComponent(L"Meshes/octaSphere.ovm"));
	sphereComponent->SetMaterial(pMaterial);

	m_pSphere->GetTransform()->Scale(15.f);
}

void SpikyScene::Update()
{
	//todo rotate on its axis
	//m_pSphere->GetTransform()->Rotate(0.f, 20.f, 0.f);
}
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
	//todo rotate on its axis
	//m_pSphere->GetTransform()->Rotate(0.f, 20.f, 0.f);
}

void SpikyScene::OnGUI()
{
	GameScene::OnGUI();
	m_pSpikyMaterial->DrawImGui();
}
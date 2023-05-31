#include "stdafx.h"
#include "PauseMenu.h"

void PauseMenu::Initialize()
{
	m_pBackground = new GameObject();
	m_pBackground->AddComponent(new SpriteComponent(L"Textures/pause.jpg", { 0.5f,0.5f }, { 1.f,1.f,1.f,0.7f }));
	AddChild(m_pBackground);

	m_pBackground->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);

	m_MainMenu = new GameObject();
	m_MainMenu->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,0.7f }));
	AddChild(m_MainMenu);

	m_MainMenu->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f - 38.f, 1.f);
	m_MainMenu->GetTransform()->Scale(0.8f, 0.7f, 0.7f);

	m_Restart = new GameObject();
	m_Restart->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));
	AddChild(m_Restart);

	m_Restart->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f + 90.f, 1.f);
	m_Restart->GetTransform()->Scale(0.8f, 0.7f, 0.7f);

	m_EndButton = new GameObject();
	m_EndButton->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));
	AddChild(m_EndButton);

	m_EndButton->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f + 220.f, 1.f);
	m_EndButton->GetTransform()->Scale(0.8f, 0.7f, 0.7f);

	const auto pMaterial = PxGetPhysics().createMaterial(.5f, .5f, .5f);

	const auto pRigidBody = m_MainMenu->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ 1.f, 0.5f, 0.5f }, *pMaterial);
}

void PauseMenu::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		if (m_MainMenu->GetComponent<SpriteComponent>()->IsMouseOverSprite())
		{
			//Load new scene
			SceneManager::Get()->SetActiveGameScene(L"Main Menu");
		}
		if (m_Restart->GetComponent<SpriteComponent>()->IsMouseOverSprite())
		{
			//restart
			SceneManager::Get()->SetActiveGameScene(L"Exam Scene");
		}
		if (m_EndButton->GetComponent<SpriteComponent>()->IsMouseOverSprite())
		{
			//Exit the game? close visual studio?
			exit(0);
		}
	}
}

void PauseMenu::OnGUI()
{
}

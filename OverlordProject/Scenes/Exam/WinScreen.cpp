#include "stdafx.h"
#include "WinScreen.h"
#include "Managers/GameSoundManager.h"

void WinScreen::Initialize()
{
	m_pBackground = new GameObject();
	m_pBackground->AddComponent(new SpriteComponent(L"Textures/winScreen.jpg", { 0.5f,0.5f }, { 1.f,1.f,1.f,0.7f }));
	AddChild(m_pBackground);

	m_pBackground->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);

	m_MainMenu = new GameObject();
	m_MainMenu->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,0.7f }));
	AddChild(m_MainMenu);

	m_MainMenu->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f + 215.f, 1.f);
	m_MainMenu->GetTransform()->Scale(0.55f, 0.55f, 0.55f);

	m_TryAgain = new GameObject();
	m_TryAgain->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));
	AddChild(m_TryAgain);

	m_TryAgain->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f + 78.f, 1.f);
	m_TryAgain->GetTransform()->Scale(0.55f, 0.55f, 0.55f);

	const auto pMaterial = PxGetPhysics().createMaterial(.5f, .5f, .5f);

	const auto pRigidBody = m_MainMenu->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ 1.f, 0.5f, 0.5f }, *pMaterial);
}

void WinScreen::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		if (m_MainMenu->GetComponent<SpriteComponent>()->IsMouseOverSprite())
		{
			//Load new scene
			GameSoundManager::Get()->Stop2DSound(GameSoundManager::Sound::WindfallIsland, true);
			GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::TitleMusic, true);
			SceneManager::Get()->SetActiveGameScene(L"Main Menu");
		}
		if (m_TryAgain->GetComponent<SpriteComponent>()->IsMouseOverSprite())
		{
			//restart
			GameSoundManager::Get()->Stop2DSound(GameSoundManager::Sound::WindfallIsland, true);
			SceneManager::Get()->SetActiveGameScene(L"Exam Scene");
		}
	}
}

void WinScreen::OnGUI()
{
}

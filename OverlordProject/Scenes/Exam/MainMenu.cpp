#include "stdafx.h"
#include "MainMenu.h"
#include <cstdlib>
#include "Managers/GameSoundManager.h"

void MainMenu::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = true;
	m_SceneContext.settings.enableOnGUI = false;

	m_pSprite = new GameObject();
	m_pSprite->AddComponent(new SpriteComponent(L"Textures/mainMenu.jpg", { 0.5f,0.5f }, { 1.f,1.f,1.f,0.7f }));
	AddChild(m_pSprite);

	m_pSprite->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);

	m_StartButton = new GameObject();
	m_StartButton->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,0.7f }));
	AddChild(m_StartButton);

	m_StartButton->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f + 425.f, m_SceneContext.windowHeight / 2.f + 66.f, 1.f);
	m_StartButton->GetTransform()->Scale(0.7f, 0.7f, 0.7f);

	m_EndButton = new GameObject();
	m_EndButton->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));
	AddChild(m_EndButton);

	m_EndButton->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f + 425.f, m_SceneContext.windowHeight / 2.f + 200.f, 1.f);
	m_EndButton->GetTransform()->Scale(0.7f, 0.7f, 0.7f);


	auto soundManager = GameSoundManager::Get();
	soundManager->AddSound(GameSoundManager::Sound::TitleMusic, "Resources/Sounds/TitleScreen.mp3");
	soundManager->Play2DSound(GameSoundManager::Sound::TitleMusic, true);
}

void MainMenu::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		if (m_StartButton->GetComponent<SpriteComponent>()->IsMouseOverSprite())
		{
			//Load new scene
			SceneManager::Get()->SetActiveGameScene(L"Control Screen");
		}
		if (m_EndButton->GetComponent<SpriteComponent>()->IsMouseOverSprite())
		{
			//Exit the game? close visual studio?
			exit(0);
		}
	}
}

void MainMenu::OnGUI()
{
}



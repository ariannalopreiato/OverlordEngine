#include "stdafx.h"
#include "ControlScreen.h"

void ControlScreen::Initialize()
{
	m_pBackground = new GameObject();
	m_pBackground->AddComponent(new SpriteComponent(L"Textures/ControlScreen.jpg", { 0.5f,0.5f }, { 1.f,1.f,1.f,0.7f }));
	AddChild(m_pBackground);

	m_pBackground->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);

	m_Continue = new GameObject();
	m_Continue->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,0.7f }));
	m_Continue->GetTransform()->Translate(m_SceneContext.windowWidth - 145.f, m_SceneContext.windowHeight / 2.f + 303.5f, 1.f);
	m_Continue->GetTransform()->Scale(0.55f, 0.5f, 0.5f);

	AddChild(m_Continue);
}

void ControlScreen::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		if (m_Continue->GetComponent<SpriteComponent>()->IsMouseOverSprite())
		{
			//Load new scene
			SceneManager::Get()->SetActiveGameScene(L"Exam Scene");
		}
	}
}

void ControlScreen::OnGUI()
{
}

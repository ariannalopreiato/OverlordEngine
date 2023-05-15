#include "stdafx.h"
#include "MainMenu.h"

void MainMenu::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = true;
	m_SceneContext.settings.enableOnGUI = false;

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Sheerwood_32.fnt");

	m_pSprite = new GameObject();
	m_pSprite->AddComponent(new SpriteComponent(L"Textures/mainmenu.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));
	AddChild(m_pSprite);

	m_pSprite->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);

	m_StartButton = new GameObject();
	m_StartButton->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));
	AddChild(m_StartButton);

	m_StartButton->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .8f);

	m_EndButton = new GameObject();
	m_EndButton->AddComponent(new SpriteComponent(L"Textures/button.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));
	AddChild(m_EndButton);

	m_EndButton->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .7f);

	const auto pMaterial = PxGetPhysics().createMaterial(.5f, .5f, .5f);

	const auto pRigidBody = m_StartButton->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ 1.f, 0.5f, 0.5f }, *pMaterial);
}

void MainMenu::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_RBUTTON))
	{
		if (const auto pPickedObject = m_SceneContext.pCamera->Pick())
		{
			RemoveChild(pPickedObject, true);
		}
	}

	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode("Start"), XMFLOAT2(50.f, 50.f), XMFLOAT4{ Colors::LightGoldenrodYellow });
}

void MainMenu::OnGUI()
{
}

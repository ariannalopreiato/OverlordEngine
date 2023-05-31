#include "stdafx.h"
#include "LifeManager.h"

LifeManager::LifeManager(int maxLife, float width, float height, float depth, float x, float y, float z)
	:LifeManager(maxLife, { width, height, depth }, {x, y, z})
{}

LifeManager::LifeManager(int maxLife, const XMFLOAT3& dimensions, const XMFLOAT3& positon)
	: m_MaxLife(maxLife)
	, m_Dimensions(dimensions)
	, m_Position(positon)
	, m_CurrentLife(maxLife)
{}

void LifeManager::Initialize(const SceneContext& /*sceneContext*/)
{
	DrawAllLives();
}

void LifeManager::Update(const SceneContext& /*sceneContext*/)
{}

void LifeManager::RemoveLife()
{
	if (m_CurrentLife > 0)
	{
		//make the heart go a lower layer
		auto transform = m_Hearts[m_CurrentLife - 1]->GetTransform();
		auto pos = transform->GetPosition();

		pos.z = m_Back;

		transform->Translate(pos);

		--m_CurrentLife;
	}
}

void LifeManager::AddLife()
{
	if (m_CurrentLife < m_MaxLife)
	{
		//make the heart go a lower layer
		auto transform = m_Hearts[m_CurrentLife]->GetTransform();
		auto pos = transform->GetPosition();

		pos.z = m_Front;

		transform->Translate(pos);

		++m_CurrentLife;
	}
}

bool LifeManager::IsDead()
{
	if (m_CurrentLife <= 0)
		return true;

	return false;
}

void LifeManager::Reset()
{
	m_CurrentLife = m_MaxLife;
	m_EmptyHearts.clear();
	m_Hearts.clear();
	DrawAllLives();
}

void LifeManager::DrawAllLives()
{
	float offset{ 1.f };

	for (int i = 0; i < m_MaxLife; ++i)
	{
		auto emptyHeart = new GameObject();
		m_EmptyHearts.push_back(emptyHeart);
		emptyHeart->AddComponent(new SpriteComponent(L"Textures/Heart/HeartEmpty.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
		AddChild(m_EmptyHearts[i]);

		auto heart = new GameObject();
		m_Hearts.push_back(heart);
		heart->AddComponent(new SpriteComponent(L"Textures/Heart/Heart.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));
		AddChild(m_Hearts[i]);

		XMFLOAT2 dimensions = emptyHeart->GetComponent<SpriteComponent>()->GetDimensions();

		m_Position.z = m_Back;

		emptyHeart->GetTransform()->Translate(m_Position);
		emptyHeart->GetTransform()->Scale(m_Dimensions);

		XMFLOAT3 frontPosition = m_Position;
		frontPosition.z = m_Front;
		heart->GetTransform()->Translate(frontPosition);
		heart->GetTransform()->Scale(m_Dimensions);

		m_Position.x = (m_Position.x + (dimensions.x * m_Dimensions.x) + offset);
	}
}

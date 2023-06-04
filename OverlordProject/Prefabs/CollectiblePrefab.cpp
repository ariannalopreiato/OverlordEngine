#include "stdafx.h"
#include "CollectiblePrefab.h"
#include "Materials/DiffuseMaterial.h"
#include "Character.h"
#include "Managers/GameSoundManager.h"

CollectiblePrefab::CollectiblePrefab(const std::wstring& model, int value, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale, float pivotOffset)
	: m_Model{ model }
	, m_Value{ value }
	, m_Position{ position }
	, m_Rotation{ rotation }
	, m_Scale{ scale }
	, m_PivotOffset{ pivotOffset }
{}

void CollectiblePrefab::Initialize(const SceneContext& /*sceneContext*/)
{
	auto& physx = PxGetPhysics();
	auto pMaterialPhys = physx.createMaterial(0.f, 0.f, 1.f);	

	AddComponent(new ModelComponent(m_Model));
	m_Position.y -= m_PivotOffset;
	GetComponent<ModelComponent>()->GetTransform()->Translate(m_Position);
	GetComponent<ModelComponent>()->GetTransform()->Scale(m_Scale);
	
	//SOUNDS
	auto soundManager = GameSoundManager::Get();
	soundManager->AddSound(GameSoundManager::Sound::Rupee, "Resources/Sounds/rupee.mp3");

	AddComponent(new RigidBodyComponent(true));
	float size{ .5f };
	GetComponent<RigidBodyComponent>()->AddCollider(PxBoxGeometry{ size / 2, size / 2, size / 2 }, *pMaterialPhys, true);
	
	SetOnTriggerCallBack([=](GameObject*, GameObject* /*otherObjectPtr*/, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
				GameSoundManager::Get()->Play2DSound(GameSoundManager::Sound::Rupee);
				m_IsCollected = true;
			}
		});
}

void CollectiblePrefab::Update(const SceneContext& sceneContext)
{
	float rotationSpeed = 90.0f; // adjust this to change the speed of rotation
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	m_Rotate += rotationSpeed * deltaTime;

	GetTransform()->Rotate(m_Rotation.x, m_Rotate, m_Rotation.z);
}

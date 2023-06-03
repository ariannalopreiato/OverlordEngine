#include "stdafx.h"
#include "CollectiblePrefab.h"
#include "Materials/DiffuseMaterial.h"
#include "Character.h"

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
	

	m_pModelMesh = AddChild(new GameObject);
	auto pModel = m_pModelMesh->AddComponent(new ModelComponent(m_Model));
	m_Position.y -= m_PivotOffset;
	pModel->GetTransform()->Translate(m_Position);
	pModel->GetTransform()->Scale(m_Scale);
	

	auto pRigidBody = m_pModelMesh->AddComponent(new RigidBodyComponent(true));
	float size{ .5f };
	pRigidBody->AddCollider(PxBoxGeometry{ size / 2, size / 2, size / 2 }, *pMaterialPhys, true);
	
	m_pModelMesh->SetOnTriggerCallBack([=](GameObject*, GameObject* /*otherObjectPtr*/, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
					m_IsCollected = true;
			}
		});
}

void CollectiblePrefab::Update(const SceneContext& sceneContext)
{
	float rotationSpeed = 60.0f; // adjust this to change the speed of rotation
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	m_Rotate += rotationSpeed * deltaTime;

	m_pModelMesh->GetTransform()->Rotate(m_Rotation.x, m_Rotate, m_Rotation.z);
}

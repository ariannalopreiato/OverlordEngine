#include "stdafx.h"
#include "CollectiblePrefab.h"
#include "Materials/DiffuseMaterial.h"

CollectiblePrefab::CollectiblePrefab(const std::wstring& texture, const std::wstring& model, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale, float pivotOffset)
	:m_Texture{ texture }
	, m_Model{ model }
	, m_Position{ position }
	, m_Rotation{ rotation }
	, m_Scale{ scale }
	, m_PivotOffset{ pivotOffset }
{}

void CollectiblePrefab::Initialize(const SceneContext& /*sceneContext*/)
{
	const auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(m_Texture);

	m_ModelMesh = AddChild(new GameObject);
	auto pModel = m_ModelMesh->AddComponent(new ModelComponent(m_Model));
	m_Position.y -= m_PivotOffset;
	pModel->GetTransform()->Translate(m_Position);
	pModel->GetTransform()->Scale(m_Scale);
	m_ModelMesh->GetTransform()->Rotate(m_Rotation);
	pModel->SetMaterial(pMaterial);
}

void CollectiblePrefab::Update(const SceneContext& /*sceneContext*/)
{
	//float rotationSpeed = 20.0f; // adjust this to change the speed of rotation
	//float deltaTime = sceneContext.pGameTime->GetElapsed();
	//m_Rotate += rotationSpeed * deltaTime;
	//m_ModelMesh->GetTransform()->Rotate(0.0f, m_Rotate, 0.0f);
}

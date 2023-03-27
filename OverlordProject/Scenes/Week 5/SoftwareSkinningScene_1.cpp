#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

void SoftwareSkinningScene_1::Initialize()
{
	auto pMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pMaterial->SetColor(XMFLOAT4{ Colors::Gray });

	GameObject* pRoot = new GameObject();
	float length{ 15.f };
	m_pBone0 = new BoneObject(pMaterial, length);
	pRoot->AddChild(m_pBone0);
	m_pBone1 = new BoneObject(pMaterial, length);
	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);
}

void SoftwareSkinningScene_1::Update()
{
	//todo: animate rotation
	if (m_AutoRotate)
	{
		m_BoneRotation = 45.f;
		m_pBone0->GetTransform()->Rotate(0.f, 0.f, m_BoneRotation);
		m_pBone1->GetTransform()->Rotate(0.f, 0.f, -m_BoneRotation * 2.f);
	}
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::PushFont(nullptr);
	ImGui::DragFloat("Rotation", &m_BoneRotation, 1.f, 0.f, 90.f);
	ImGui::Checkbox("Automatic Mode", &m_AutoRotate);
	ImGui::PopFont();
}
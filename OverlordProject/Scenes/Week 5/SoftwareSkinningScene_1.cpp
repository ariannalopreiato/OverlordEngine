#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

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
	if (m_AutoRotate)
	{
		m_BoneRotation += m_RotationSign * m_RotationAngle * m_SceneContext.pGameTime->GetElapsed();

		//cheange rotation sign when it reaches the max or min point
		if (m_RotationSign > 0 && m_BoneRotation >= m_RotationAngle)
		{
			m_BoneRotation -= m_BoneRotation - m_RotationAngle;
			m_RotationSign = -1;
		}
		else if (m_RotationSign < 0 && m_BoneRotation <= -m_RotationAngle)
		{
			m_BoneRotation += m_BoneRotation + m_RotationAngle;
			m_RotationSign = 1;
		}

		m_pBone0->GetTransform()->Rotate(0.f, 0.f, m_BoneRotation);
		m_pBone1->GetTransform()->Rotate(0.f, 0.f, -m_BoneRotation * 2.f);	
	}
	else
	{
		m_pBone0->GetTransform()->Rotate(0.f, 0.f, m_Bone0Rotation);
		m_pBone1->GetTransform()->Rotate(0.f, 0.f, -m_Bone1Rotation * 2.f);
	}
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::Checkbox("Auto Rotate", &m_AutoRotate);

	ImGui::SliderFloat("Bone One Rotation", &m_Bone0Rotation, 0.f, 90.f);
	ImGui::SliderFloat("Bone Two Rotation", &m_Bone1Rotation, -45.f, 45.f);
}
#include "stdafx.h"
#include "BoneObject.h"
#include "Materials/ColorMaterial.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: m_pMaterial(pMaterial)
	, m_pLength(length) 
{}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty = AddChild(new GameObject());
	const auto pModel = pEmpty->AddComponent(new ModelComponent(L"Meshes/Bone.ovm"));
	pModel->SetMaterial(m_pMaterial);
	pEmpty->GetTransform()->Rotate(0.f, -90.f, 0.f);
	//auto currentScale = pEmpty->GetTransform()->GetScale();
	pEmpty->GetTransform()->Scale(XMFLOAT3{ m_pLength,  m_pLength , m_pLength });
}

void BoneObject::AddBone(BoneObject* pBone)
{
	//translate next to parent bone
	pBone->GetTransform()->Translate(m_pLength, 0.f, 0.f);
	this->AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	auto matrix = XMLoadFloat4x4(&this->GetTransform()->GetWorld());
	auto inverse = XMMatrixInverse(nullptr, matrix);
	XMStoreFloat4x4(&m_BindPose, inverse);
	auto children = this->GetChildren<BoneObject>();

	for (const auto child : children)
	{		
		auto matrixChild = XMLoadFloat4x4(&child->GetTransform()->GetWorld());
		auto inverseChild = XMMatrixInverse(nullptr, matrixChild);
		XMStoreFloat4x4(&child->m_BindPose, inverseChild);
	}
}

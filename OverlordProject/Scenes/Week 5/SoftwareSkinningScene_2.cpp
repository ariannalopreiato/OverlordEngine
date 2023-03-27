#include "stdafx.h"
#include "SoftwareSkinningScene_2.h"
#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

void SoftwareSkinningScene_2::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion
}

void SoftwareSkinningScene_2::Initialize()
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

	//todo ??
	m_pBone0->CalculateBindPose();
	GameObject* pBoxDrawer = new GameObject();
	AddChild(pBoxDrawer);
	m_pMeshDrawer = pBoxDrawer->AddComponent(new MeshDrawComponent(24, true));
	InitializeVertices(length);
}

void SoftwareSkinningScene_2::Update()
{
	if (m_AutoRotate)
	{
		m_BoneRotation = 45.f;
		m_pBone0->GetTransform()->Rotate(0.f, 0.f, m_BoneRotation);
		m_pBone1->GetTransform()->Rotate(0.f, 0.f, -m_BoneRotation * 2.f);
	}

	XMMATRIX bone0Transform = XMMatrixMultiply(XMLoadFloat4x4(&m_pBone0->GetBindPose()), XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld()));
	XMMATRIX bone1Transform = XMMatrixMultiply(XMLoadFloat4x4(&m_pBone1->GetBindPose()), XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld()));

	for (int i = 0; i < m_SkinnedVertices.size(); ++i)
	{
		auto v = XMFLOAT3(m_SkinnedVertices[i].originalVertex.Position.x, m_SkinnedVertices[i].originalVertex.Position.y, m_SkinnedVertices[i].originalVertex.Position.z);
		XMVECTOR vertex = XMLoadFloat3(&v);
		XMFLOAT3 out;
	
		
		if (i < 24)
			XMStoreFloat3(&out, XMVector3TransformCoord(vertex, bone0Transform));
		else
			XMStoreFloat3(&out, XMVector3TransformCoord(vertex, bone1Transform));

		m_SkinnedVertices[i].transformedVertex.Position = out;
	}

	m_pMeshDrawer->RemoveTriangles();
	QuadPosNormCol quad;
	for (int i = 0; i < m_SkinnedVertices.size(); ++i)
	{
		quad.Vertex1 = m_SkinnedVertices[i].transformedVertex;
		quad.Vertex2 = m_SkinnedVertices[i + 1].transformedVertex;
		quad.Vertex3 = m_SkinnedVertices[i + 2].transformedVertex;
		quad.Vertex4 = m_SkinnedVertices[i + 3].transformedVertex;

		m_pMeshDrawer->AddQuad(quad);
		i += 3;
	}

	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_2::OnGUI()
{
}

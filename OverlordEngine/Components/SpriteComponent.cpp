#include "stdafx.h"
#include "SpriteComponent.h"
#include "./Graphics/SpriteRenderer.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Draw(const SceneContext& sceneContext)
{
	if (!m_pTexture)
		return;

	//TODO_W4(L"Draw the sprite with SpriteRenderer::Draw")

	auto spriteRenderer = SpriteRenderer::Get();

	const auto& transform = m_pGameObject->GetTransform();
	const auto& worldMatrix = transform->GetWorld();
	XMMATRIX matrix = XMLoadFloat4x4(&worldMatrix);
	DirectX::XMVECTOR position, rotation, scale;
	DirectX::XMMatrixDecompose(&scale, &rotation, &position, matrix);

	XMFLOAT4 rot;
	XMStoreFloat4(&rot, rotation);
	const auto eulerRotation = MathHelper::QuaternionToEuler(rot);

	spriteRenderer->Draw(sceneContext);
		//m_pTexture,
		//position.m128_f32[0], position.m128_f32[1], position.m128_f32[2],
		//eulerRotation.z,
		//scale.m128_f32[0], scale.m128_f32[1]	
	
	//Here you need to draw the SpriteComponent using the Draw of the sprite renderer
	// The sprite renderer is a singleton
	// you will need to position (X&Y should be in screenspace, Z contains the depth between [0,1]), the rotation and the scale from the owning GameObject
	// You can use the MathHelper::QuaternionToEuler function to help you with the z rotation 
}

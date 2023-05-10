#include "stdafx.h"
#include "Ladder.h"
#include "CubePrefab.h"
#include "Character.h"

Ladder::Ladder(float width, float height, float depth, const XMFLOAT3& position, const XMFLOAT3& rotation)
	:m_Width(width)
	, m_Height(height)
	, m_Depth(depth)
	, m_Position(position)
	, m_Rotation(rotation)
{}

Ladder::Ladder(const XMFLOAT3& dimensions, const XMFLOAT3& position, const XMFLOAT3& rotation)
	:Ladder(dimensions.x, dimensions.y, dimensions.z, position, rotation)
{}

void Ladder::Initialize(const SceneContext&)
{
	auto& physx = PxGetPhysics();
	auto pMaterial = physx.createMaterial(0.f, 0.f, 1.f);
	m_pLadder = new CubePrefab(m_Width, m_Height, m_Depth, XMFLOAT4{ Colors::Transparent });
	AddChild(m_pLadder);
	m_pLadder->GetTransform()->Translate(m_Position);
	m_pLadder->GetTransform()->Rotate(m_Rotation);
	auto pRigidBody = m_pLadder->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ m_Width / 2, m_Height / 2, m_Depth / 2 }, *pMaterial, true);

	m_pLadder->SetOnTriggerCallBack([=](GameObject*, GameObject* /*otherObjectPtr*/, PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
				//if (otherObjectPtr == m_pPlayer)
				m_pPlayer->SetIsClimbing(true);
			}
			if (triggerAction == PxTriggerAction::LEAVE)
			{
				//if (otherObjectPtr == m_pPlayer)
				m_pPlayer->SetIsClimbing(false);
			}
		});
}
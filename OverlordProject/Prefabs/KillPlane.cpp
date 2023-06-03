#include "stdafx.h"
#include "KillPlane.h"
#include "Character.h"

KillPlane::KillPlane(Character* pPlayer, const XMFLOAT3& position, const XMFLOAT3& scale)
	: m_pPlayer(pPlayer)
	, m_Position(position)
	, m_Scale(scale)
{
}

void KillPlane::Initialize(const SceneContext& /*sceneContext*/)
{
	auto& physx = PxGetPhysics();
	auto pMaterialPhys = physx.createMaterial(0.f, 0.f, 1.f);


	auto pMesh = AddChild(new GameObject);
	auto pModel = pMesh->AddComponent(new ModelComponent(L"Meshes/UnitPlane.ovm"));
	pModel->GetTransform()->Translate(m_Position);
	pModel->GetTransform()->Scale(m_Scale);


	auto pRigidBody = pMesh->AddComponent(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxBoxGeometry{ m_Scale.x / 2, 0.25f, m_Scale.z / 2 }, *pMaterialPhys, true);

	pMesh->SetOnTriggerCallBack([=](GameObject*, GameObject* , PxTriggerAction triggerAction)
		{
			if (triggerAction == PxTriggerAction::ENTER)
			{
					m_IsHit = true;
			}
		});
}

void KillPlane::Update(const SceneContext& /*sceneContext*/)
{
}

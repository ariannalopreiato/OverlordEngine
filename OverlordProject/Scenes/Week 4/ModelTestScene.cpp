#include "stdafx.h"
#include "ModelTestScene.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Components/ModelComponent.h"

void ModelTestScene::Initialize()
{
	auto& physx = PxGetPhysics();
	auto defaultMaterial = physx.createMaterial(0.5f, 0.5f, 0.1f);
	
	//GORUNDPLANE
	GameSceneExt::CreatePhysXGroundPlane(*this, defaultMaterial);

	GameObject* pChair{ nullptr };
	DiffuseMaterial* pChairMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();

	pChair = AddChild(new GameObject());
	const auto chairComponent = pChair->AddComponent(new ModelComponent(L"Meshes/chair.ovm"));
	chairComponent->SetMaterial(pChairMaterial);

	//pChairMaterial->SetColor(XMFLOAT4{ Colors::BlueViolet });
	pChairMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	const auto pChairRigidBody = pChair->AddComponent(new RigidBodyComponent());
	pChairRigidBody->AddCollider(PxConvexMeshGeometry{ ContentManager::Load<PxConvexMesh>(L"Meshes/chair.ovpc") }, *defaultMaterial);
	pChairRigidBody->SetConstraint(RigidBodyConstraint::TransY, false);
	pChair->GetTransform()->Translate(0.f, 0.5f, 0.f);
}

#include "stdafx.h"
#include "CameraMovement.h"
#include "Scenegraph/GameObject.h"

CameraMovement::CameraMovement(GameObject* player)
	:m_pPlayer(player)
{
    m_CurrentDistance = 10.f;
}

void CameraMovement::Initialize(const SceneContext& /*sceneContext*/)
{
}

void CameraMovement::Update(const SceneContext& /*sceneContext*/)
{
	
}

void CameraMovement::LateUpdate(const SceneContext& /*sceneContext*/)
{
    // Define a vector representing the positive z-axis of the world
    XMVECTOR playerForward = XMLoadFloat3(&m_pPlayer->GetTransform()->GetForward());

    // Calculate the vector pointing from the camera to the player
    m_VectorToPlayer = (XMLoadFloat3(&GetTransform()->GetPosition()) - XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()));

    GetTransform()->Translate((-playerForward) * m_CurrentDistance);

    // Calculate the angle between the positive z-axis and the vector pointing to the player
    //XMVECTOR angle = XMVector3AngleBetweenVectors(XMLoadFloat3(&world), m_VectorToPlayer);

    //XMFLOAT3 rotation;
    //XMStoreFloat3(&rotation, angle);

    //auto characterPos = m_pPlayer->GetTransform()->GetPosition();

    //GetTransform()->Rotate(characterPos.x, characterPos.y, 0.f);
}

void CameraMovement::CameraRotation()
{

}

void CameraMovement::FollowPlayer()
{

}

void CameraMovement::GetCloserToPlayer()
{

}
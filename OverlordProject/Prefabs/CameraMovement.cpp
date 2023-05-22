#include "stdafx.h"
#include "CameraMovement.h"
#include "Scenegraph/GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Prefabs/Character.h"

CameraMovement::CameraMovement(GameObject* player, XMFLOAT3 cameraOffset)
	: m_pPlayer(player)
    , m_CameraOffset(cameraOffset)
{
    m_CurrentDistance = 10.f;
    XMFLOAT3 worldForward{ 0, 0, -1 };
}

void CameraMovement::Initialize(const SceneContext& /*sceneContext*/)
{
    KeepPlayerCentered();
    //FollowPlayer();
}

void CameraMovement::Update(const SceneContext& /*sceneContext*/)
{
   
}

void CameraMovement::LateUpdate(const SceneContext& sceneContext)
{
    CameraRotation(sceneContext);
    KeepPlayerCentered();   
    //FollowPlayer();
}

void CameraMovement::CameraRotation(const SceneContext& sceneContext)
{
    //set the rotation direction (left = 1 , right = -1)
    m_RotationDirection = 0.f;

    if (sceneContext.pInput->IsActionTriggered(actionId_MoveLeft))
        m_RotationDirection += 1.f;

    if (sceneContext.pInput->IsActionTriggered(actionId_MoveRight))
        m_RotationDirection += -1.f;


    //calculate the rotation angle (rotation speed * dt * rotation direction)
    float alpha = m_RotatingSpeed * sceneContext.pGameTime->GetElapsed() * m_RotationDirection;

    //get the rotation matrix
    auto rotationMatrix = XMMatrixRotationY(alpha);

    //get the current vector from the player to the camera
    auto current = XMVectorSubtract(XMLoadFloat3(&GetTransform()->GetPosition()), XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()));

    //get the new vector
    XMVECTOR target = XMVector3Transform(current, rotationMatrix);

    //get the new camera position
    auto newPos = XMVectorAdd(target, XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()));

    GetTransform()->Translate(newPos);
}

void CameraMovement::KeepPlayerCentered()
{
    //vector between the player position and the camera position
    XMVECTOR forward = XMVectorSubtract(XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()), XMLoadFloat3(&GetTransform()->GetPosition()));
    forward = XMVector3Normalize(forward);

    //get the world up vector
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //get the right vector to the current camera
    XMVECTOR right = XMVector3Cross(up, forward);
    right = XMVector3Normalize(right);

    //get the up vector of the camera
    up = XMVector3Cross(forward, right);
    up = XMVector3Normalize(up);

    XMMATRIX viewMatrix = XMMatrixLookToLH(XMLoadFloat3(&GetTransform()->GetPosition()), forward, up);

    XMVECTOR rotationQuaternion = XMQuaternionRotationMatrix(viewMatrix);
    rotationQuaternion = XMQuaternionConjugate(rotationQuaternion);
    GetTransform()->Rotate(rotationQuaternion);
}

void CameraMovement::GetCloserToPlayer()
{

}

void CameraMovement::FollowPlayer()
{
    XMVECTOR offset = XMLoadFloat3(&m_CameraOffset);
    auto playerPos = XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition());
    GetTransform()->Translate(playerPos + offset);
}

void CameraMovement::SetCameraMovement(int moveLeft, int moveRight)
{
    actionId_MoveLeft = moveLeft;
    actionId_MoveRight = moveRight;
}

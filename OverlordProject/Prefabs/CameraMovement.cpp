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
    //m_CurrentDistance = 10.f;
    m_CameraDistance = 10.f;
    m_CameraHeight = cameraOffset.y;
    XMFLOAT3 worldForward{ 0, 0, -1 };
}

void CameraMovement::Initialize(const SceneContext& /*sceneContext*/)
{
    FollowPlayer();
    KeepPlayerCentered();
    GetCloserToPlayer();
}

void CameraMovement::Update(const SceneContext& /*sceneContext*/)
{
}

void CameraMovement::LateUpdate(const SceneContext& sceneContext)
{
    FollowPlayer();
    CameraRotation(sceneContext);
    KeepPlayerCentered();
    /*GetCloserToPlayer();*/
}

void CameraMovement::CameraRotation(const SceneContext& sceneContext)
{
    //set the rotation direction (left = 1 , right = -1)
    m_RotationDirection = 0.f;

    if (sceneContext.pInput->IsActionTriggered(actionId_MoveLeft))
        m_RotationDirection += 1.f;

    if (sceneContext.pInput->IsActionTriggered(actionId_MoveRight))
        m_RotationDirection += -1.f;

    if (m_RotationDirection != 0.f)
    {//calculate the rotation angle (rotation speed * dt * rotation direction)
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

void CameraMovement::GetCloserToPlayer(CollisionGroup ignoreGroups)
{
    auto playerPos = m_pPlayer->GetTransform()->GetPosition(); 
    PxVec3 rayStart = { playerPos.x, playerPos.y, playerPos.z }; 
    auto currDistance = XMVectorSubtract(XMLoadFloat3(&playerPos), XMLoadFloat3(&GetTransform()->GetPosition()));
    XMFLOAT3 rayDirection;
    XMStoreFloat3(&rayDirection, currDistance);

    PxVec3 direction = PxVec3({ rayDirection.x, rayDirection.y, rayDirection.z }).getNormalized();

    PxQueryFilterData filterData{};
    filterData.data.word0 = ~UINT(ignoreGroups);

    PxRaycastBuffer hit{};
    if (m_pScene->GetPhysxProxy()->Raycast(rayStart, direction, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData))
    {
        auto pos = hit.block.position;
        GetTransform()->Translate(pos.x, pos.y, pos.z);
    }
    //return nullptr;

    //if hit, get hit pos and move camera to that position + small offset
}

void CameraMovement::FollowPlayer()
{  
    auto pos = GetTransform()->GetPosition();
    pos.y -= m_CameraHeight;

    //vector between the camera position and the player position
    auto currDistance = XMVectorSubtract(XMLoadFloat3(&pos), XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()));
    float currLength;
    XMStoreFloat(&currLength, XMVector3Length(currDistance));
    
    if (currLength > m_CameraDistance)
    {
        currDistance = XMVector3Normalize(currDistance);
        auto newVec = currDistance * m_CameraDistance;
        XMVECTOR output = XMVectorAdd(newVec, XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()));
        XMFLOAT3 newPos;
        XMStoreFloat3(&newPos, output);
        newPos.y += m_CameraHeight;
        GetTransform()->Translate(newPos);
    }
}

void CameraMovement::SetCameraMovement(int moveLeft, int moveRight)
{
    actionId_MoveLeft = moveLeft;
    actionId_MoveRight = moveRight;
}
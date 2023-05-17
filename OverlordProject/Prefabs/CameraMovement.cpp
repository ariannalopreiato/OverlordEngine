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
    FollowPlayer();
}

void CameraMovement::Update(const SceneContext& /*sceneContext*/)
{
	
}

void CameraMovement::LateUpdate(const SceneContext& /*sceneContext*/)
{
    KeepPlayerCentered();
    FollowPlayer();
}

void CameraMovement::CameraRotation()
{

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
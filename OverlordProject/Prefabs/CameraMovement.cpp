#include "stdafx.h"
#include "CameraMovement.h"
#include "Scenegraph/GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Prefabs/Character.h"

CameraMovement::CameraMovement(GameObject* player)
	:m_pPlayer(player)
{
    m_CurrentDistance = 10.f;
}

void CameraMovement::Initialize(const SceneContext& /*sceneContext*/)
{
    FollowPlayer();
}

void CameraMovement::Update(const SceneContext& /*sceneContext*/)
{
	
}

void CameraMovement::LateUpdate(const SceneContext& /*sceneContext*/)
{
    FollowPlayer();
}

void CameraMovement::CameraRotation()
{

}

void CameraMovement::FollowPlayer()
{
    XMVECTOR playerForward = XMLoadFloat3(&m_pPlayer->GetTransform()->GetForward());

    m_VectorToPlayer = (XMLoadFloat3(&GetTransform()->GetPosition()) - XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()));

    auto pos = m_pPlayer->GetTransform()->GetPosition();
    if (!dynamic_cast<Character*>(m_pPlayer)) return;

    auto player = dynamic_cast<Character*>(m_pPlayer);

    auto forward = player->GetVelocity();
    if (forward.x == 0 && forward.z == 0) return;

    XMFLOAT3 camToPlayer = { GetTransform()->GetPosition().x - m_pPlayer->GetTransform()->GetPosition().x,
    GetTransform()->GetPosition().y - m_pPlayer->GetTransform()->GetPosition().y ,
    GetTransform()->GetPosition().z - m_pPlayer->GetTransform()->GetPosition().z };
    float angle = float(atan2(-camToPlayer.x, -camToPlayer.z) * 180 / M_PI);

    GetTransform()->Rotate(30, angle, 0);


    auto forwardLength = sqrtf(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    XMFLOAT3 forwardNormalized = { forward.x / forwardLength, forward.y / forwardLength, forward.z / forwardLength };
    const auto length = 6;
    pos.y += 3;
    pos.x -= forwardNormalized.x * length;
    pos.z -= forwardNormalized.z * length;

    GetTransform()->Translate(pos);
}

void CameraMovement::GetCloserToPlayer()
{

}
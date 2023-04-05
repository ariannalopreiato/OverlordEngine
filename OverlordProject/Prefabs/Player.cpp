#include "stdafx.h"
#include "Player.h"
#include "Materials/DiffuseMaterial_Skinned.h"

Player::Player(const std::wstring& texture, const std::wstring& model)
	: m_Texture(texture)
	, m_Model(model)
{}

void Player::Initialize()
{
	//m_SceneContext.settings.enableOnGUI = true;

	const auto pSkinnedMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pSkinnedMaterial->SetDiffuseTexture(m_Texture);

	const auto pObject = AddChild(new GameObject);
	auto pModel = pObject->AddComponent(new ModelComponent(m_Model));
	pModel->SetMaterial(pSkinnedMaterial);

	pObject->GetTransform()->Scale(0.15f);
	//pObject->GetTransform()->Translate(0.f, 5.f, 0.f);


	pAnimator = pModel->GetAnimator();
	//pAnimator->SetAnimation(m_AnimationClipId);
	pAnimator->SetAnimation(1);
	pAnimator->SetAnimationSpeed(m_AnimationSpeed);

	//Gather Clip Names
	m_ClipCount = pAnimator->GetClipCount();
	m_ClipNames = new char* [m_ClipCount];
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		auto clipName = StringUtil::utf8_encode(pAnimator->GetClip(static_cast<int>(i)).name);
		const auto clipSize = clipName.size();
		m_ClipNames[i] = new char[clipSize + 1];
		strncpy_s(m_ClipNames[i], clipSize + 1, clipName.c_str(), clipSize);
	}

	pAnimator->Play();
}

void Player::Update()
{
}

Player::~Player()
{
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		delete[] m_ClipNames[i];
	}

	delete[] m_ClipNames;
}
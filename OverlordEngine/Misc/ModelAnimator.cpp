#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	m_AnimationClips = m_pMeshFilter->GetAnimationClips();
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		auto passedTicks = m_AnimationSpeed * sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond;
		passedTicks = std::max(std::min(passedTicks, m_CurrentClip.duration), 0.f);

		//2. 	
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
				m_TickCount += m_CurrentClip.duration;
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration)
				m_TickCount -= m_CurrentClip.duration;
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;

		for (int i = 0; i < m_CurrentClip.keys.size(); ++i)
		{
			if (m_CurrentClip.keys[i].tick > m_TickCount)
			{
				keyA = m_CurrentClip.keys[i - 1];
				keyB = m_CurrentClip.keys[i];
				break;
			}
		}

		//4.
		auto blendFactor = (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick);
		m_Transforms.clear();
		for (int i = 0; i < m_pMeshFilter->m_BoneCount; ++i)
		{
			auto boneTransformA = keyA.boneTransforms[i];
			XMMATRIX transformA = XMLoadFloat4x4(&boneTransformA);
			auto boneTransformB = keyB.boneTransforms[i];
			XMMATRIX transformB = XMLoadFloat4x4(&boneTransformB);

			XMVECTOR scaleA;
			XMVECTOR rotA;
			XMVECTOR transA;
			XMMatrixDecompose(&scaleA, &rotA, &transA, transformA);

			XMVECTOR scaleB;
			XMVECTOR rotB;
			XMVECTOR transB;
			XMMatrixDecompose(&scaleB, &rotB, &transB, transformB);

			auto scale = XMVectorLerp(scaleA, scaleB, blendFactor);
			auto rotation = XMQuaternionSlerp(rotA, rotB, blendFactor);
			auto translation = XMVectorLerp(transA, transB, blendFactor);
			
			auto translationM = XMMatrixTranslationFromVector(translation);
			auto scaleM = XMMatrixScalingFromVector(scale);
			auto rotationM = XMMatrixRotationQuaternion(rotation);
			auto matrix = scaleM * rotationM * translationM;
			XMFLOAT4X4 outMatrix;
			XMStoreFloat4x4(&outMatrix, matrix);
			m_Transforms.emplace_back(outMatrix);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	m_ClipSet = false;
	for (const auto clip : m_AnimationClips)
	{
		if (clip.name == clipName)
		{
			SetAnimation(clip);
			break;
		}
	}

	Reset();
	Logger::LogError(L"Clip couldn't be found.");
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	m_ClipSet = false;
	if (clipNumber < m_AnimationClips.size())
	{
		auto clip = m_AnimationClips[clipNumber];
		SetAnimation(clip);
	}
	else
	{
		Reset();
		Logger::LogError(L"Clip number out of bounds.");
		return;
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;
	Reset(false);
}

bool ModelAnimator::IsAnimationFinished() const
{
	return m_TickCount >= m_CurrentClip.duration/2; 
}

void ModelAnimator::Reset(bool pause)
{
	if (pause)
		m_IsPlaying = false;

	m_TickCount = 0.f;
	m_AnimationSpeed = 1.f;

	if (m_ClipSet)
	{
		auto boneTransforms = m_CurrentClip.keys[0].boneTransforms;
		m_Transforms.assign(boneTransforms.begin(), boneTransforms.end());
	}
	else
	{
		auto identityMatrix = XMMatrixIdentity();
		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, identityMatrix);
		
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, matrix);
	}
}

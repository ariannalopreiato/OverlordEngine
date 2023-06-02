#include "stdafx.h"
#include "SkyboxMaterial.h"

SkyboxMaterial::SkyboxMaterial()
	:Material<SkyboxMaterial>(L"Effects/Skybox.fx")
{}

void SkyboxMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void SkyboxMaterial::InitializeEffectVariables()
{
}

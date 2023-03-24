#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial()
	:Material<SpikyMaterial>(L"Effects/SpikyShader.fx")
{}

void SpikyMaterial::InitializeEffectVariables()
{
}

//void SpikyMaterial::SetDiffuseTexture(const std::wstring& assetFile)
//{
//	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
//	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
//}

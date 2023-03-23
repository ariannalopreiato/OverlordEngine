#include "stdafx.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial() 
	:Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fx")
{}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	const auto diffuse = GetVariable(assetFile);
	ContentManager::Load<Material>(assetFile);
}

void DiffuseMaterial::InitializeEffectVariables()
{
}

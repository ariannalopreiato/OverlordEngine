#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial()
	:Material<UberMaterial>(L"Effects/UberShader.fx")
{}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gNormalMap", m_pNormalTexture);
}

void UberMaterial::SetEnvironmentTexture(const std::wstring& assetFile)
{
	m_pEnvironmentTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gEnvironmentMap", m_pEnvironmentTexture);
}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gLightDirection", XMFLOAT4{ -0.577f, -0.577f, 0.577f, 1.f });
	SetVariable_Vector(L"gColorAmbient", XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4{ 0.9f, 0.f, 0.f, 1.f });
	SetVariable_Vector(L"gColorSpecular", XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
	SetVariable_Vector(L"gColorFresnel", XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Scalar(L"gUseTextureNormal", true);
	SetVariable_Scalar(L"gUseTextureSpecular", true);
	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Scalar(L"gUseEnvironmentMapping", true);
	SetVariable_Scalar(L"gUseOpacityMap", false);
	SetVariable_Scalar(L"gUseBlinn", true);
	SetVariable_Scalar(L"gUsePhong", true);
	SetVariable_Scalar(L"gAmbientIntensity", 0.f);
	SetVariable_Scalar(L"gShininess", 50.f);
	SetVariable_Scalar(L"gSpecularIntensity", 1.f);
	SetVariable_Scalar(L"gFresnelPower", 1.f);
	SetVariable_Scalar(L"gFresnelMultiplier", 1.f);
	SetVariable_Scalar(L"gFresnelHardness", 1.f);
	SetVariable_Scalar(L"gReflectionStrength", 1.0f);
	SetVariable_Scalar(L"gRefractionStrength", 1.0f);
	SetVariable_Scalar(L"gRefractionIndex", 1.0f);
}

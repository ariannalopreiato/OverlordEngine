#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial()
	:Material<UberMaterial>(L"Effects/UberShader.fx")
{}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseTexture", m_pDiffuseTexture);
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gNormalTexture", m_pNormalTexture);
}

void UberMaterial::SetEnvironmentTexture(const std::wstring& assetFile)
{
	m_pEnvironmentTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureEnv", m_pEnvironmentTexture);
}

void UberMaterial::SetSpecularTexture(const std::wstring& assetFile)
{
	m_pSpecularTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gSpecularTexture", m_pSpecularTexture);
}

void UberMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacityTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gOpacityTexture", m_pOpacityTexture);
}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gLightDirection", XMFLOAT4{ 0.551f, -0.534f, 0.641f, 1.f });
	SetVariable_Scalar(L"gUseTextureDiffuse", true);	
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4{ 0.9f, 0.f, 0.f, 1.f });
	SetVariable_Vector(L"gColorSpecular", XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });

	SetVariable_Scalar(L"gUseTextureSpecular", true);
	SetVariable_Scalar(L"gShininess", 40.f);

	SetVariable_Vector(L"gColorAmbient", XMFLOAT4{ 0.f, 0.f, 0.f, 1.f });
	SetVariable_Scalar(L"gAmbientIntensity", 0.f);

	SetVariable_Scalar(L"gFlip", true);
	SetVariable_Scalar(L"gUseNormalTexture", true);
	SetVariable_Scalar(L"gUseEnvironmentMapping", true);

	SetVariable_Scalar(L"gReflectionStrength", 0.8f);
	SetVariable_Scalar(L"gRefractionStrength", 0.5f);
	SetVariable_Scalar(L"gRefractionIndex", 0.9f);

	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Vector(L"gColorFresnel", XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
	
	SetVariable_Scalar(L"gFresnelPower", 2.f);
	SetVariable_Scalar(L"gFresnelMultiplier", 1.5f);
	SetVariable_Scalar(L"gFresnelHardness", 2.f);
	SetVariable_Scalar(L"gSpecularIntensity", 1.f);
	
	
	SetVariable_Scalar(L"gUseOpacityMap", false);
	SetVariable_Scalar(L"gUseBlinn", false);
	SetVariable_Scalar(L"gUsePhong", true);

}

#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial()
	:Material<SpikyMaterial>(L"Effects/SpikyShader.fx")
{}

void SpikyMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"m_LightDirection", XMFLOAT4{ 0.577f, -0.577f, 0.577f, 1.f });
	SetVariable_Scalar(L"gSpikeLength", 0.2f);
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4{ 1.f, 1.f, 1.f, 1.f });
}
#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	//TODO_W9(L"Implement Destructor")
	delete[] m_ParticlesArray;
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Initialize")
	if (!m_pParticleMaterial)
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();

	CreateVertexBuffer(sceneContext);

	//Use the ContentManager to load the particle texture and store it in m_pParticleTexture
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement CreateVertexBuffer")
	if (m_pVertexBuffer)
		SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC buffer;
	buffer.Usage = D3D11_USAGE_DYNAMIC;
	buffer.ByteWidth = m_ParticleCount * sizeof(VertexParticle);
	buffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer.MiscFlags = 0;
	
	HRESULT hr = sceneContext.d3dContext.pDevice->CreateBuffer(&buffer, nullptr, &m_pVertexBuffer);
	HANDLE_ERROR(hr, L"Failed to create vertex buffer.")
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Update")
	float particleInterval = (m_EmitterSettings.maxEnergy + m_EmitterSettings.minEnergy) / float(m_ParticleCount);
	float elapsed = sceneContext.pGameTime->GetElapsed();

	m_LastParticleSpawn += elapsed;

	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE resource;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	
	auto* pVertexParticle = static_cast<VertexParticle*>(resource.pData);

	for (UINT i = 0; i < m_ParticleCount; ++i)
	{
		if (m_ParticlesArray[i].isActive)
			UpdateParticle(m_ParticlesArray[i], elapsed);

		else
		{
			if(m_LastParticleSpawn >= particleInterval)
				SpawnParticle(m_ParticlesArray[i]);
		}

		if (m_ParticlesArray[i].isActive)
		{
			pVertexParticle[m_ActiveParticles] = m_ParticlesArray[i].vertexInfo;
			++m_ActiveParticles;
		}
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	//TODO_W9(L"Implement UpdateParticle")
	if (!p.isActive)
		return;

	p.currentEnergy -= elapsedTime;
	if (p.currentEnergy <= 0)
	{
		p.isActive = false;
		return;
	}

	auto pos = XMLoadFloat3(&p.vertexInfo.Position);
	auto vel = XMLoadFloat3(&m_EmitterSettings.velocity);
	auto finalPos = pos + vel * elapsedTime;
	XMStoreFloat3(&p.vertexInfo.Position, finalPos);

	float lifePercent = p.currentEnergy / p.totalEnergy;

	p.vertexInfo.Color = m_EmitterSettings.color;
	p.vertexInfo.Color.w = m_EmitterSettings.color.w * lifePercent * 2;

	if (p.sizeChange < 1)
	{
		p.vertexInfo.Size = p.initialSize + (1 - lifePercent) * p.sizeChange;
	}
	else if(p.sizeChange > 1)
	{
		p.vertexInfo.Size = p.initialSize * 2;
	}
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	//TODO_W9(L"Implement SpawnParticle")
	p.isActive = true;
	p.totalEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);

	XMVECTOR randomDirection = { 1,0,0 };
	auto randomRotationMatrix = XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI));
	randomDirection = XMVector3TransformNormal(randomDirection, randomRotationMatrix);

	float randomDistance = MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius);

	XMFLOAT3 position;
	auto pos = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	XMStoreFloat3(&position, pos + randomDirection * randomDistance);
	p.vertexInfo.Position = position;

	p.initialSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.vertexInfo.Size = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);

	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement PostDraw")
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

	auto context = m_pParticleMaterial->GetTechniqueContext();
	sceneContext.d3dContext.pDeviceContext->IASetInputLayout(context.pInputLayout);
	sceneContext.d3dContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(VertexParticle);
	UINT offset{};
	sceneContext.d3dContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC desc{};
	context.pTechnique->GetDesc(&desc);

	for (uint32_t i = 0; i < desc.Passes; ++i)
	{
		context.pTechnique->GetPassByIndex(i)->Apply(0, sceneContext.d3dContext.pDeviceContext);
		sceneContext.d3dContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}
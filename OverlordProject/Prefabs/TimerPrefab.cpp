#include "stdafx.h"
#include "TimerPrefab.h"

TimerPrefab::TimerPrefab(int totalTime, XMFLOAT2 timerPosition)
	: m_TotalTime(totalTime)
	, m_Position(timerPosition)
{}

void TimerPrefab::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Sheerwood_32.fnt");
	m_CurrentTime = float(m_TotalTime);
}


void TimerPrefab::ShowTime()
{
	int minutes = int(m_CurrentTime / 60); // Calculate the minutes
	int remainingSeconds = int(m_CurrentTime) % 60; // Calculate the remaining seconds

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << remainingSeconds;

	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(oss.str()), m_Position, XMFLOAT4{ Colors::LightGoldenrodYellow });
}

bool TimerPrefab::IsTimeOut()
{
	if (m_CurrentTime == 0)
		return true;
	
	return false;
}

void TimerPrefab::Update(const SceneContext& sceneContext)
{
	m_CurrentTime -= sceneContext.pGameTime->GetElapsed();

	if (m_CurrentTime <= 0)
		m_CurrentTime = 0;

	ShowTime();
}


void TimerPrefab::Reset()
{
	m_CurrentTime = float(m_TotalTime);
	ShowTime();
}
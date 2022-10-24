// COMP710 GP Framework 2022

#include "scenegame.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>
#include <engine/input.h>

#include <cassert>
#include <utils.h>
#include <cmath>
#include <imgui/imgui_impl_sdl.h>

#include "ship.h"
#include "shot.h"
#include "rock.h"

SceneGame::SceneGame()
	: m_iScore(0)
{
#ifdef _DEBUG
	dm_name = "Game Scene";
#endif // _DEBUG
}

SceneGame::~SceneGame()
{

}

bool SceneGame::Initialise(Renderer& renderer, SceneManager& sceneManager, FMOD::System& fmodSystem)
{
	m_pShip = new Ship();
	m_pShip->Initialise(renderer, fmodSystem);

	Rock* rock = new Rock();
	rock->Initialise(renderer, fmodSystem);
	m_pRocks.push_back(rock);

#ifdef _DEBUG
	AddEntityToDebugQueue(m_pShip);
	AddEntityToDebugQueue(rock);
#endif // _DEBUG

	return Scene::Initialise(renderer, sceneManager, fmodSystem);
}

void SceneGame::Process(float deltaTime, Input& input)
{

	m_pShip->Process(deltaTime, input);

	// This is not good, very bad, but we're just gonna make this work now
	if (m_pRocks.size() > 0)
	{
		for (std::vector<Rock*>::iterator rock = m_pRocks.begin(); rock != m_pRocks.end(); ++rock)
		{
			(*rock)->Process(deltaTime, input);
			if (m_pShots.size() > 0)
			{
				for (std::vector<Shot*>::iterator shot = m_pShots.begin(); shot != m_pShots.end(); ++shot)
				{
					if ((*rock)->IsColliding(**shot))
					{
						++m_iScore;
#ifdef _DEBUG
						RemoveEntityFromDebugQueue(*shot);
						RemoveEntityFromDebugQueue(*rock);
#endif // _DEBUG

						shot = m_pShots.erase(shot);
						rock = m_pRocks.erase(rock);
					}
				}
			}

			if (m_pRocks.size() == 0)
			{
				break;
			}
		}
	}

	for (std::vector<Shot*>::iterator shot = m_pShots.begin(); shot != m_pShots.end(); ++shot)
	{
		(*shot)->Process(deltaTime, input);
	}

	if (m_pShip->HasShot())
	{
		Shot* shot = CreateShot();
		shot->Setup(m_pShip->GetPosition(), 270.f - m_pShip->GetAngle());
#ifdef _DEBUG
		AddEntityToDebugQueue(shot);
#endif // _DEBUG
		m_pShots.push_back(shot);
	}
}

void SceneGame::Draw(Renderer& renderer)
{
	char scoreText[16];
	sprintf_s(scoreText, "Score: %d", m_iScore);
	Vector2 scoreTextPos = { renderer.GetWidth() / 2.f, 10 };
	renderer.DrawDynamicText(scoreText, FontSize::MEDIUM, scoreTextPos, { 255, 255, 255, 255 }, SpriteOrigin::TOPCENTER);

	m_pShip->Draw(renderer);

	for (std::vector<Rock*>::iterator rock = m_pRocks.begin(); rock != m_pRocks.end(); ++rock)
	{
		(*rock)->Draw(renderer);
	}

	for (std::vector<Shot*>::iterator shot = m_pShots.begin(); shot != m_pShots.end(); ++shot)
	{
		(*shot)->Draw(renderer);
	}


}

#ifdef _DEBUG
void SceneGame::DebugDraw()
{
	Scene::BeginDebugDraw();
	ImGui::SliderInt("Score", (int*)&m_iScore, 0, 100);
	ImGui::End();
}
#endif // _DEBUG

Shot* SceneGame::CreateShot()
{
	Shot* shot = new Shot();
	shot->Initialise(*m_pRenderer, *m_pFMODsystem);
	return shot;
}
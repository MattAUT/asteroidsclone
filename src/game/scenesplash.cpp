// COMP710 GP Framework 2022

#include "scenesplash.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/scenemanager.h>
#include <engine/input.h>

#include <cassert>
#include <utils.h>
#include <cmath>
#include <imgui/imgui_impl_sdl.h>

SceneSplash::SceneSplash()
	: m_fTime(-0.4f) 
	, m_fSceneLength(10)
{
#ifdef _DEBUG
	dm_name = "Splash Screen Scene";
#endif // _DEBUG
}

SceneSplash::~SceneSplash()
{
	m_fSceneLength = 0.0f;

	delete m_pAUTlogo;
	delete m_pFMODlogo;
	delete m_pSDLlogo;
	delete m_pOPENGLlogo;

	m_pAUTlogo = nullptr;
	m_pFMODlogo = nullptr;
	m_pSDLlogo = nullptr;
	m_pOPENGLlogo = nullptr;
}

bool SceneSplash::Initialise(Renderer& renderer, SceneManager& sceneManager, FMOD::System& fmodSystem)
{
	m_pAUTlogo = renderer.CreateSprite("assets\\sprites\\logos\\aut.png", SpriteOrigin::CENTER);
	m_pAUTlogo->SetScale(0.4f);
	m_pAUTlogo->SetAlpha(0.f);

	m_pFMODlogo = renderer.CreateSprite("assets\\sprites\\logos\\fmod.png", SpriteOrigin::CENTER);
	m_pFMODlogo->SetScale(0.4f);

	m_pSDLlogo = renderer.CreateSprite("assets\\sprites\\logos\\sdl.jpg", SpriteOrigin::CENTER);
	m_pSDLlogo->SetScale(0.5f);

	m_pOPENGLlogo = renderer.CreateSprite("assets\\sprites\\logos\\opengl.png", SpriteOrigin::CENTER);
	m_pOPENGLlogo->SetScale(0.3f);


	return Scene::Initialise(renderer, sceneManager, fmodSystem);
}

void SceneSplash::Process(float deltaTime, Input& input)
{
	m_fTime += deltaTime;

	if (input.GetKeyState(SDL_SCANCODE_SPACE) == BS_PRESSED)
	{
		Skip();
	}

	float autAlpha = EaseInOutQuad(m_fTime - m_fSceneLength / 2.f, 0, 1, m_fSceneLength / 4.f);
	m_pAUTlogo->SetAlpha(autAlpha);

	float libAplha = EaseInOutQuad(m_fTime, 0, 1, m_fSceneLength / 4.f);
	m_pFMODlogo->SetAlpha(libAplha);
	m_pSDLlogo->SetAlpha(libAplha);
	m_pOPENGLlogo->SetAlpha(libAplha);

	if (m_fTime > m_fSceneLength)
	{
		m_pSceneManager->SwitchScenes(1);
	}
}

void SceneSplash::Draw(Renderer& renderer)
{
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();


	Vector2 logoAUTpos = { SCREEN_WIDTH / 2.f , SCREEN_HEIGHT / 2.f };
	m_pAUTlogo->Draw(renderer, logoAUTpos);


	Vector2 logoFMODpos = { SCREEN_WIDTH / 3.f - (m_pFMODlogo->GetWidth() / 2.f) - 30 , SCREEN_HEIGHT / 2.f };
	m_pFMODlogo->Draw(renderer, logoFMODpos);

	Vector2 logoSDLDpos = { SCREEN_WIDTH / 2.f , SCREEN_HEIGHT / 2.f };
	m_pSDLlogo->Draw(renderer, logoSDLDpos);

	Vector2 logoOPENGLpos = { SCREEN_WIDTH - (SCREEN_WIDTH / 3.f) + (m_pOPENGLlogo->GetWidth() / 2.f) - 30, SCREEN_HEIGHT / 2.f };
	m_pOPENGLlogo->Draw(renderer, logoOPENGLpos);
}

#ifdef _DEBUG
void SceneSplash::DebugDraw()
{
	ImGui::Begin("Splash Screen");
	if (ImGui::Button("Reset Scene"))
	{
		m_fTime = 0;
	}

	if (ImGui::Button("Skip"))
	{
		Skip();
	}

	ImGui::SliderFloat("Time", &m_fTime, 0.f, m_fSceneLength);
	ImGui::SliderFloat("Scene Length", &m_fSceneLength, 0.f, 20.f);

	ImGui::End();
}
#endif // _DEBUG

void
SceneSplash::Skip()
{
	m_fTime = m_fSceneLength;
}
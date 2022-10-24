// COMP710 GP Framework 2022
#include "game.h"

#include <engine/renderer.h>
#include <engine/scene.h>
#include <engine/scenemanager.h>
#include <engine/input.h>
#include <engine/xinputcontroller.h>
#include <fmod.hpp>

#include <cstdio>
#include <cassert>
#include <ctime>
#include <logmanager.h>
#include <ini.h>
#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

// Scenes
#include "scenesplash.h"
#include "scenegame.h"

// Static Members:
Game *Game::sm_pInstance = 0;

Game &Game::GetInstance() {
    if (sm_pInstance == 0) {
        sm_pInstance = new Game();
    }

    return (*sm_pInstance);
}

void Game::DestroyInstance() {
    delete sm_pInstance;
    sm_pInstance = 0;
}

Game::Game()
    : m_pRenderer(0)
	, m_bLooping(true)
	, m_fExecutionTime(0),
      m_fElapsedSeconds(0)
	, m_iFrameCount(0)
	, m_fFrametimeUpdate(0.25f)
	, m_bShowDebugWindow(false)
	, m_bPaused(false)
{

}

Game::~Game() {
	m_pFMODsystem->release();

    delete m_pRenderer;
    delete m_pInput;
	delete m_pSceneManager;

    m_pRenderer = NULL;
    m_pInput = NULL;
	m_pSceneManager = NULL;
}

void 
Game::Quit() 
{ 
	m_bLooping = false; 
}

void 
Game::Pause() 
{ 
	m_bPaused = !m_bPaused; 
}

void
Game::ToggleDebugWindow() 
{
    m_bShowDebugWindow = !m_bShowDebugWindow;
    m_pInput->ShowMouseCursor(m_bShowDebugWindow);
}

bool 
Game::Initialise()
{
	srand(time(NULL));

    int bbWidth = 1280;
    int bbHeight = 960;

    m_pRenderer = new Renderer();
    if (!m_pRenderer->Initialise(true, bbWidth, bbHeight))
	{
        LogManager::GetInstance().Log("Renderer failed to initialise!");
        return false;
    }

    bbWidth = m_pRenderer->GetWidth();
    bbHeight = m_pRenderer->GetHeight();
    m_iLastTime = SDL_GetPerformanceCounter();
    m_pRenderer->SetClearColour(0, 0, 0);

    m_pInput = new Input();
    m_pInput->Initialise();

	if (FMOD::System_Create(&m_pFMODsystem) != FMOD_OK)
	{
		LogManager::GetInstance().Log("Could not create FMOD instance!");
		assert(0);
	}

	if (m_pFMODsystem->init(8, FMOD_INIT_NORMAL, NULL))
	{
		LogManager::GetInstance().Log("FMOD failed to initalise!");
		assert(0);
	}

	assert(m_pFMODsystem);
	LogManager::GetInstance().Log("FMOD Initialised.");

	m_pSceneManager = new SceneManager();
	Scene* scenes[SceneManager::sm_uiSceneCount] = { new SceneSplash(), new SceneGame() };
	m_pSceneManager->Initialise(scenes, *m_pRenderer, *m_pFMODsystem);

    return true;
}

bool 
Game::DoGameLoop() 
{
    const float stepSize = 1.0f / 200.0f;

    m_pInput->ProcessInput();
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
#ifdef _DEBUG
        ImGuiIO &io = ImGui::GetIO();
        ImGui_ImplSDL2_ProcessEvent(&event);
#endif // _DEBUG
    }

	XInputController *controller = m_pInput->GetController(0);

    if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        Quit();
    }
#ifdef _DEBUG
	bool debugKeyPressed =
		m_pInput->GetKeyState(SDL_SCANCODE_GRAVE) == BS_PRESSED;

		bool controllerDebugKeyPresses = m_pInput->GetNumberOfControllersAttached() > 0 ? 
			controller->GetButtonState(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == BS_PRESSED : 
			false;

	if (debugKeyPressed || controllerDebugKeyPresses) {
		ToggleDebugWindow();
	}
#endif // _DEBUG
	bool pauseKeyPressed =
		m_pInput->GetKeyState(SDL_SCANCODE_ESCAPE) == BS_PRESSED;
	bool controllerPauseKeyPressed = m_pInput->GetNumberOfControllersAttached() > 0 ?
		controller->GetButtonState(SDL_CONTROLLER_BUTTON_START) == BS_PRESSED :
		false;

	if (pauseKeyPressed || controllerPauseKeyPressed) {
		Pause();
	}

    if (m_bLooping) {
        Uint64 current = SDL_GetPerformanceCounter();
        float deltaTime = (current - m_iLastTime) /
                          static_cast<float>(SDL_GetPerformanceFrequency());
        m_iLastTime = current;

        m_fExecutionTime += deltaTime;

        Process(deltaTime);

#ifdef USE_LAG
        m_fLag += deltaTime;

        int innerLag = 0;

        while (m_fLag >= stepSize) {
            Process(stepSize);

            m_fLag -= stepSize;

            ++m_iUpdateCount;
            ++innerLag;
        }
#endif // USE_LAG

		ProcessFrameCounting(deltaTime);
        Draw(*m_pRenderer);
    }

    return m_bLooping;
}

void 
Game::Process(float deltaTime) 
{
    
    m_pSceneManager->GetActiveScene()->Process(!m_bPaused ? deltaTime : 0, *m_pInput);
	m_pFMODsystem->update();
}

void
Game::Draw(Renderer &renderer) 
{
    ++m_iFrameCount;

    renderer.Clear();

	m_pSceneManager->GetActiveScene()->Draw(renderer);

#ifdef _DEBUG
    DebugDraw(renderer);
#endif // _DEBUG

    renderer.Present();
}

#ifdef _DEBUG
void 
Game::DebugDraw(Renderer &renderer)
{
    if (m_bShowDebugWindow) {
		// On Screen Text
        char fpsText[16];
		Vector2 fpsPosition = { 6, 0 };
        sprintf_s(fpsText, "FPS: %d", m_iFPS);
        renderer.DrawDynamicText(fpsText, FontSize::SMALL, fpsPosition, {255, 255, 255, 255}, SpriteOrigin::TOPLEFT);

		char frameTimeText[32];
		Vector2 frameTimePosition = { 6, 24 };
		sprintf_s(frameTimeText, "Frame Times: %fms", 1.f / m_iFPS);
		renderer.DrawDynamicText(frameTimeText, FontSize::SMALL, frameTimePosition, { 255, 255, 255, 255 }, SpriteOrigin::TOPLEFT);

		if (m_bPaused)
		{
			Vector2 pausePosition = { m_pRenderer->GetWidth() / 2.f, m_pRenderer->GetHeight() / 2.f };
			// Drawing dynamic text is not efficent currently, but that should change when
			// ASCII tiling is implemented
			renderer.DrawDynamicText("PASUED", FontSize::LARGE, pausePosition, { 255, 255, 255, 255 }, SpriteOrigin::CENTER);
		}

		// ImGui
        bool open = true;
        ImGui::Begin("Application", &open, ImGuiWindowFlags_MenuBar);
        ImGui::Text("Engine Framework (COMP710)");
        if (ImGui::Button("Quit")) {
            Quit();
        }
        if (ImGui::Button("Pause")) {
            Pause();
        }

		ImGui::Separator();
		ImGui::Spacing();
		LogManager::GetInstance().DebugDraw();

        ImGui::End();

		m_pRenderer->DebugDraw();
		m_pSceneManager->GetActiveScene()->DebugDraw();
    }
}
#endif // _DEBUG

void 
Game::ProcessFrameCounting(float deltaTime) 
{
    // Count total simulation time elapsed:
    m_fElapsedSeconds += deltaTime;

    // Frame Counter:
    if (m_fElapsedSeconds > m_fFrametimeUpdate) {
        m_fElapsedSeconds -= m_fFrametimeUpdate;
        m_iFPS = static_cast<int>(m_iFrameCount / m_fFrametimeUpdate);
        m_iFrameCount = 0;
    }
}

// COMP710 GP Framework 2022
#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

// Forward declarations:
class Renderer;
class Scene;
class SceneManager;
class Input;

namespace FMOD 
{
	class System;
}

class Game
{
    // Member methods:
public:
    static Game& GetInstance();
    static void DestroyInstance();

    bool Initialise();
    bool DoGameLoop();
    void Quit();
	void Pause();

	void ToggleDebugWindow();

protected:
    void Process(float deltaTime);
    void Draw(Renderer& renderer);
#ifdef _DEBUG
	void DebugDraw(Renderer& renderer);
#endif // _DEBUG

    void ProcessFrameCounting(float deltaTime);

private:
    Game();
    ~Game();
    Game(const Game& game);
	Game& operator=(const Game& game) {}

    // Member data:
public:

protected:
    static Game* sm_pInstance;
    Renderer* m_pRenderer;
	Input* m_pInput;
	SceneManager* m_pSceneManager;
	FMOD::System* m_pFMODsystem;

    __int64 m_iLastTime;
    float m_fExecutionTime;
    float m_fElapsedSeconds;
    int m_iFrameCount;
    int m_iFPS;
	float m_fFrametimeUpdate;

#ifdef USE_LAG
    float m_fLag;
    int m_iUpdateCount;
#endif // USE_LAG

    bool m_bLooping;
	bool m_bShowDebugWindow;
	bool m_bPaused;

private:

};

#endif // __GAME_H__

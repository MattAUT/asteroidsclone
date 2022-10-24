#include "scenemanager.h"

#include "renderer.h"
#include "scene.h"
#include "logmanager.h"

SceneManager::SceneManager()
	: m_iActiveScene(0)
{

}

SceneManager::~SceneManager()
{
	for (unsigned int i = 0; i < sm_uiSceneCount; ++i)
	{
		delete m_pScenes[i];
		m_pScenes[i] = nullptr;
	}
}

bool
SceneManager::Initialise(Scene** scenes, Renderer& renderer, FMOD::System& fmodSystem)
{
	for (unsigned int i = 0; i < sm_uiSceneCount; ++i)
	{
		if (!scenes[i]->Initialise(renderer, *this, fmodSystem))
		{
			LogManager::GetInstance().Error("SceneManager failed to initalise a scene!");
			continue;
		}

		m_pScenes[i] = scenes[i];
	}

	LogManager::GetInstance().Log("SceneManager Initalised");
	return true;
}

bool
SceneManager::SwitchScenes(unsigned int index)
{
	if (index > sm_uiSceneCount - 1)
	{
		return false;
	}

	m_iActiveScene = index;
	return true;
}

Scene* 
SceneManager::GetActiveScene()
{
	return m_pScenes[m_iActiveScene];
}
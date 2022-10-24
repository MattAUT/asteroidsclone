#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

class Renderer;
class Scene;

namespace FMOD
{
	class System;
}

#include <vector>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	bool Initialise(Scene** scenes, Renderer& renderer, FMOD::System& fmodSystem);
	bool SwitchScenes(unsigned int index);

	Scene* GetActiveScene();

protected:
private:
	SceneManager(const SceneManager& s);
	SceneManager& operator=(const SceneManager& s) {}

public:
	static const unsigned int sm_uiSceneCount = 2;
protected:
	Scene* m_pScenes[sm_uiSceneCount];
	unsigned int m_iActiveScene;
private:
};

#endif // __SCENEMANAGER_H__
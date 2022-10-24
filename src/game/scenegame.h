#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include <engine/scene.h>
#include <vector>

// Forward Declarations
class SceneManager;
class Renderer;
class Sprite;
class Input;
class Ship;
class Shot;
class Rock;

namespace FMOD
{
	class System;
	class Channel;
	class Sound;
}

class SceneGame : public Scene
{
	// Member Methods
public:
	SceneGame();
	virtual ~SceneGame();

	virtual bool Initialise(Renderer& renderer, SceneManager& sceneManager, FMOD::System& fmodSystem) override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif // _DEBUG

	Shot* CreateShot();

protected:
private:
	SceneGame(const SceneGame& s);
	SceneGame& operator=(const SceneGame& s) {}

	// Member Data
public:
protected:
	Ship* m_pShip;
	unsigned int m_iScore;
	std::vector<Shot*> m_pShots;
	std::vector<Rock*> m_pRocks;
private:

};

#endif // __SCENEGAME_H__


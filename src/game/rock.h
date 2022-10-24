#ifndef __ROCK_H__
#define __ROCK_H__

#include <engine/entity.h>

class Sprite;

class Rock : public Entity
{
	// Member Methods
public:
	Rock();
	virtual ~Rock();

	virtual bool Initialise(Renderer& renderer, FMOD::System& fmodSystem) override;
	virtual void Reset() override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif // _DEBUG

protected:

private:
	Rock(const Rock& r);
	Rock& operator=(const Rock& r) {}

	// Member Data
public:
protected:
	Sprite* m_pSprite;
	float m_fSize;

private:

};

#endif // __ROCK_H__


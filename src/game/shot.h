#ifndef __SHOT_H__
#define __SHOT_H__

#include <engine/entity.h>

class Sprite;

class Shot : public Entity
{
	// Member Methods
public:
	Shot();
	virtual ~Shot();

	virtual bool Initialise(Renderer& renderer, FMOD::System& fmodSystem) override;
	virtual void Reset() override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif // _DEBUG

	void Setup(Vector2 position, float angle);

protected:

private:
	Shot(const Shot& s);
	Shot& operator=(const Shot& s) {}

	// Member Data
public:
protected:
	Sprite* m_pSprite;

private:

};

#endif // __SHOT_H__


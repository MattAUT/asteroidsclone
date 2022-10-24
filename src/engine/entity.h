#ifndef __ENTITY_H__
#define __ENTITY_H__

class Renderer;
class Input;
class ID;

namespace FMOD
{
	class System;
}

#include "id.h"
#include "vector2.h"
#include <string>


class Entity
{
	// Methods
public:
	virtual ~Entity();
	// Generates any static assets revelant to the specific entity. Parent
	// method should be called before child method, to ensure entities are
	// not re-initalised
	virtual bool Initialise(Renderer& renderer, FMOD::System& fmodSystem);
	// Initialises any dynamic variables that might change over the course
	// of runtime. Should be called during entity initialisation.
	virtual void Reset() = 0;
	virtual void Process(float deltaTime, Input& input) = 0;
	virtual void Draw(Renderer& renderer) = 0;
#ifdef _DEBUG
	virtual void DebugDraw() = 0;
#endif // _DEBUG

	bool IsColliding(Entity& other);
	bool IsLoaded() const;
	void Unload();
	Vector2& GetPosition();
	Vector2& GetVelocity();
	ID GetId() const;
	std::string GetIdAsString();

protected:
	Entity();

	float& GetCollisionRadius();

private:
	Entity(const Entity& e);
	Entity& operator=(const Entity& e) {}

	// Data:
public:
protected:
	ID m_id;
	Vector2 m_position;
	Vector2 m_velocity;
	float m_fCollisionRadius;
	bool m_bInitalised;
	bool m_bIsLoaded;

private:
};

#endif // __ENTITY_H__
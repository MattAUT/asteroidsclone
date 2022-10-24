#include "entity.h"

#include "renderer.h"
#include "logmanager.h"

#include <cstdio>
#include <limits>
#include <utils.h>

Entity::Entity()
	: m_velocity{ 0, 0 }
	, m_bInitalised(false)
	, m_bIsLoaded(false)
{
	m_id = ID();
	LogManager::GetInstance().Log("Entity Generated with ID: " + m_id.AsString());
}

Entity::~Entity()
{
	
}

bool 
Entity::Initialise(Renderer& renderer, FMOD::System& fmodSystem)
{
	if (m_bInitalised)
	{
		std::string message = "Attemped to re-initalise entity {ID:" + m_id.AsString() + "}";
		LogManager::GetInstance().Error(message);
		return false;
	}

	return true;
}

bool
Entity::IsColliding(Entity& other)
{
	Vector2 diff = m_position - other.GetPosition();
	float distance = diff.Length();

	return distance < m_fCollisionRadius + other.GetCollisionRadius();
}

bool
Entity::IsLoaded() const
{
	return m_bIsLoaded;
}

void 
Entity::Unload()
{
	m_bIsLoaded = false;
}

Vector2&
Entity::GetPosition()
{
	return m_position;
}

Vector2&
Entity::GetVelocity()
{
	return m_velocity;
}

// TODO: find out how to make this const
ID
Entity::GetId() const
{
	return m_id;
}

std::string
Entity::GetIdAsString()
{
	return m_id.AsString();
}

float&
Entity::GetCollisionRadius()
{
	return m_fCollisionRadius;
}
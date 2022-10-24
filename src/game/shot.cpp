#include "shot.h"

#include <engine/renderer.h>
#include <engine/sprite.h>

#include <cmath>
#include <utils.h>
#include <vector2.h>
#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

Shot::Shot()
{
	m_fCollisionRadius = 5.f;
}

Shot::~Shot()
	
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

bool 
Shot::Initialise(Renderer& renderer, FMOD::System& fmodSystem)
{
	if (!Entity::Initialise(renderer, fmodSystem))
	{
		return false;
	}

	m_pSprite = renderer.CreateSprite("assets\\sprites\\shot.png", SpriteOrigin::CENTER);
	
	Reset();
	return true;
}

void Shot::Reset()
{
	m_pSprite->SetScale(2.f);
	m_bIsLoaded = true;
}

void 
Shot::Setup(Vector2 position, float angle)
{
	m_position = position;
	m_pSprite->SetAngle(90.f - angle);
	m_velocity = { DegToRad(angle), 1000.f, COORD_SYSTEM::POLAR };
}

void
Shot::Process(float deltaTime, Input& input)
{
	m_position += m_velocity * deltaTime;
}

void 
Shot::Draw(Renderer& renderer)
{
	m_pSprite->Draw(renderer, m_position);
}

#ifdef _DEBUG
void 
Shot::DebugDraw()
{

}
#endif // _DEBUG
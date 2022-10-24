#include "rock.h"

#include <engine/renderer.h>
#include <engine/sprite.h>

#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

Rock::Rock()
	: m_fSize(5.f)
{

}

Rock::~Rock()
{

}

bool 
Rock::Initialise(Renderer& renderer, FMOD::System& fmodSystem)
{
	if (!Entity::Initialise(renderer, fmodSystem))
	{
		return false;
	}

	m_pSprite = renderer.CreateSprite("assets\\sprites\\rock.png", SpriteOrigin::CENTER);

	Reset();
	return true;
}

void
Rock::Reset()
{
	m_position = { 100, 100 };
	m_fCollisionRadius = m_fSize / 2.f * m_pSprite->GetWidth();
	m_bIsLoaded = true;
}

void 
Rock::Process(float deltaTime, Input& input)
{
	static const float rotationSpeed = 45.f;
	m_pSprite->SetScale(m_fSize);
	m_pSprite->SetAngle(m_pSprite->GetAngle() + (rotationSpeed * deltaTime));
}

void 
Rock::Draw(Renderer& renderer)
{
	m_pSprite->Draw(renderer, m_position);
}

#ifdef _DEBUG
void 
Rock::DebugDraw()
{
	ImGui::Text("Entity Type: Rock");
	ImGui::Separator();

	ImGui::Text("Position: {%f, %f}", m_position.x, m_position.y);
}
#endif // _DEBUG
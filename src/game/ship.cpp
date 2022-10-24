#include "ship.h"

#include <engine/renderer.h>
#include <engine/sprite.h>
#include <engine/spritesheetanimation.h>
#include <engine/input.h>
#include <engine/xinputcontroller.h>
#include <fmod.hpp>

#include <cmath>
#include <limits>
#include <utils.h>
#include <vector2.h>
#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif // _DEBUG

#include "shot.h"

Ship::Ship()
	: m_fAngle(0.f)
	, m_fDeadzone(0.8f)
	, m_fThrottleInput(0.f)
	, m_fMaxSpeed(250.f)
	, m_fAcceleration(0.65f)
	, m_fBoosterVolume(0.5f)
	, m_fShootVolume(0.8f)
	, m_bHasShot(false)
	, m_acceleration{ 0, 0 }
{
	m_fCollisionRadius = 5.f;
}

Ship::~Ship()
	
{
	delete m_pShipSprite;
	delete m_pFlames;

	m_pShipSprite = nullptr;
	m_pFlames = nullptr;
}

bool 
Ship::Initialise(Renderer& renderer, FMOD::System& fmodSystem)
{
	if (!Entity::Initialise(renderer, fmodSystem))
	{
		return false;
	}

	m_pFMODsystem = &fmodSystem;
	fmodSystem.createSound("assets\\sound\\boosterloop.wav", FMOD_LOOP_NORMAL, NULL, &m_pBoosterLoop);
	fmodSystem.createSound("assets\\sound\\boosterattack.wav", FMOD_DEFAULT, NULL, &m_pBoosterAttack);
	fmodSystem.createSound("assets\\sound\\boosterdecay.wav", FMOD_DEFAULT, NULL, &m_pBoosterDecay);
	fmodSystem.createSound("assets\\sound\\shoot.wav", FMOD_DEFAULT, NULL, &m_pShoot);

	m_pShipSprite = renderer.CreateSprite("assets\\sprites\\ship.png", SpriteOrigin::CENTER);
	m_pFlames = renderer.CreateAnimatedSprite("assets\\sprites\\fire.png");
	m_position = { renderer.GetWidth() / 2.f, renderer.GetHeight() / 2.f };

	Reset();
	return true;
}

void
Ship::Reset()
{
	m_pShipSprite->SetScale(2.f);

	m_pFlames->SetupFrames(15, 20, SpriteOrigin::BOTTOMCENTER);
	m_pFlames->SetFrameDuration(0.08f);
	m_pFlames->SetScale(1.5f);
	m_pFlames->SetAngle(180.f);
	m_pFlames->Animate();
	m_pFlames->SetLooping(true);

	m_bIsLoaded = true;
}

void
Ship::Process(float deltaTime, Input& input)
{
	m_pFlames->Process(deltaTime);

	static Vector2 rotationInput = { 0.f, 0.f };
	static float shootInput = 0.f;
	static XInputController* controller = input.GetController(0);

	// Get Inputs
	if (input.GetNumberOfControllersAttached() > 0)
	{
		rotationInput = controller->GetLeftStick() / static_cast<float>(std::numeric_limits<int16_t>::max());
		m_fThrottleInput = controller->GetRightTrigger() / static_cast<float>(std::numeric_limits<int16_t>::max());
		shootInput = controller->GetLeftTrigger() / static_cast<float>(std::numeric_limits<int16_t>::max());
	}
	

	// Shooting
	static bool resetShotSound = false;
	m_bHasShot = false;

	if ((!resetShotSound && shootInput) || controller->GetButtonState(SDL_CONTROLLER_BUTTON_X) == BS_PRESSED)
	{
		m_pFMODsystem->playSound(m_pShoot, NULL, false, &m_pShootChannel);
		m_pShootChannel->setVolume(m_fShootVolume);
		m_bHasShot = true;
		resetShotSound = true;
	}

	if (!shootInput)
	{
		resetShotSound = false;
	}

	// Ship Movement
	if (rotationInput.LengthSquared() > powf(m_fDeadzone, 2))
	{
		m_fAngle = AngleClamp(RadToDeg(-atan2f(rotationInput.y, rotationInput.x) - static_cast<float>(M_PI) / 2.f));	
	}

	m_acceleration = Vector2(DegToRad(-m_fAngle - 90.f), m_fThrottleInput * m_fAcceleration, COORD_SYSTEM::POLAR) * deltaTime;
	m_velocity = Vector2::Clamp(0, m_velocity + m_acceleration, 1);
	
	m_pShipSprite->SetAngle(m_fAngle);
	m_pFlames->SetAngle(m_fAngle + 180.f);

	m_position += m_velocity * deltaTime * m_fMaxSpeed;

	// Ship Movement Sound
	static bool resetBoosterSound = true;

	bool boosterLoopIsPlaying = false;
	bool boosterAttackIsPlaying = false;
	m_pBoosterLoopChannel->isPlaying(&boosterLoopIsPlaying);
	m_pBoosterAttackChannel->isPlaying(&boosterAttackIsPlaying);

	if (m_fThrottleInput && !boosterLoopIsPlaying)
	{
		m_pBoosterDecayChannel->stop();

		if (!boosterAttackIsPlaying)
		{
			if (resetBoosterSound)
			{
				m_pFMODsystem->playSound(m_pBoosterAttack, NULL, false, &m_pBoosterAttackChannel);
				m_pBoosterAttackChannel->setVolume(m_fBoosterVolume);
				resetBoosterSound = false;
			}
			else
			{
				m_pFMODsystem->playSound(m_pBoosterLoop, NULL, false, &m_pBoosterLoopChannel);
				m_pBoosterLoopChannel->setVolume(m_fBoosterVolume);
			}
		}
	}

	if (!m_fThrottleInput && (boosterLoopIsPlaying || boosterAttackIsPlaying))
	{
		m_pBoosterLoopChannel->stop();
		m_pBoosterAttackChannel->stop();
		m_pFMODsystem->playSound(m_pBoosterDecay, NULL, false, &m_pBoosterDecayChannel);
		m_pBoosterDecayChannel->setVolume(m_fBoosterVolume);
		resetBoosterSound = true;
	}
}

void 
Ship::Draw(Renderer& renderer)
{

	if (m_fThrottleInput)
	{
		float flamesDistance = 2.5f;
		Vector2 flamesPos =
		{
			m_position.x + -flamesDistance * 2 * cosf(-DegToRad(m_fAngle + 90.f)),
			m_position.y + -flamesDistance * 2 * sinf(-DegToRad(m_fAngle + 90.f))
		};

		m_pFlames->Draw(renderer, flamesPos);
	}
	
	m_pShipSprite->Draw(renderer, m_position);
}

#ifdef _DEBUG
void 
Ship::DebugDraw()
{
	ImGui::Text("Entity Type: Ship");
	ImGui::Separator();

	ImGui::Text("Ship Movement");
	ImGui::Text("Throttle: {%f}", m_fThrottleInput);
	ImGui::Text("Angle: {%f}", &m_fAngle);
	ImGui::Text("Acceleration Vector: {%f, %f}", m_acceleration.x, m_acceleration.y);
	ImGui::Text("Velocity: {%f, %f}", m_velocity.x, m_velocity.y);
	ImGui::Text("Position: {%f, %f}", m_position.x, m_position.y);
	ImGui::SliderFloat("Max Speed", &m_fMaxSpeed, 50.f, 500.f);
	ImGui::SliderFloat("Acceleration", &m_fAcceleration, 0.1f, 2.f);
	ImGui::Separator();

	ImGui::Text("Ship Sound");
	ImGui::SliderFloat("Booster Volume", &m_fBoosterVolume, 0.f, 2.f);
	ImGui::SliderFloat("Shot Volume", &m_fShootVolume, 0.f, 2.f);
}
#endif // _DEBUG

bool
Ship::HasShot() const
{
	return m_bHasShot;
}

float
Ship::GetAngle() const
{
	return m_fAngle;
}
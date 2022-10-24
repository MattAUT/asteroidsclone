#ifndef __SHIP_H__
#define __SHIP_H__

#include <engine/entity.h>

class Sprite;
class SpriteSheetAnimation;

namespace FMOD
{
	class Sound;
	class Channel;
};

class Ship : public Entity
{
	// Member Methods
public:
	Ship();
	virtual ~Ship();

	virtual bool Initialise(Renderer& renderer, FMOD::System& fmodSystem) override;
	virtual void Reset() override;
	virtual void Process(float deltaTime, Input& input) override;
	virtual void Draw(Renderer& renderer) override;
#ifdef _DEBUG
	virtual void DebugDraw() override;
#endif // _DEBUG

	bool HasShot() const;
	float GetAngle() const;

protected:

private:
	Ship(const Ship& s);
	Ship& operator=(const Ship& s) {}

	// Member Data
public:
protected:
	Sprite* m_pShipSprite;
	SpriteSheetAnimation* m_pFlames;
	FMOD::System* m_pFMODsystem;

	bool m_bHasShot;

	Vector2 m_acceleration;

	float m_fAngle;
	float m_fDeadzone;
	float m_fThrottleInput;

	float m_fMaxSpeed;
	float m_fAcceleration;

	FMOD::Sound* m_pBoosterLoop;
	FMOD::Channel* m_pBoosterLoopChannel;
	FMOD::Sound* m_pBoosterAttack;
	FMOD::Channel* m_pBoosterAttackChannel;
	FMOD::Sound* m_pBoosterDecay;
	FMOD::Channel* m_pBoosterDecayChannel;
	FMOD::Sound* m_pShoot;
	FMOD::Channel* m_pShootChannel;

	float m_fBoosterVolume;
	float m_fShootVolume;
private:

};

#endif // __SHIP_H__


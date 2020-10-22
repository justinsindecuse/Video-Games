#pragma once
#include "Component.h"
#include "Math.h"
#include "CollisionComponent.h"

class CameraComponent : public Component {
public:
	CameraComponent(Actor* owner);
	float mPitchAngle = 0.0f;
	float GetPitchSpeed() { return mPitchSpeed; }
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	float currentRoll = 0.0f;
private:
	void Update(float deltaTime);
	float mPitchSpeed = 0.0f;
};
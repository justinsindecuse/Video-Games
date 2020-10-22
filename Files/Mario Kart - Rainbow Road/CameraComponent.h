#pragma once
#include "Component.h"
#include "Math.h"
#include "CollisionComponent.h"

class CameraComponent : public Component {
public:
	CameraComponent(Actor* owner);
	float horizontalDist = 60.0f;
	float targetOffset = 50.0f;
	float springConstant = 256.0f;
	float dampConstant;
	Vector3 cameraVel;
	Vector3 cameraPos;
	Vector3 ideal();
	void rotateCam(CollSide side);
	void SnapToIdeal();
private:
	void Update(float deltaTime);
};
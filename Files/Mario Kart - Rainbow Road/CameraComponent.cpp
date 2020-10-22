#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"

CameraComponent::CameraComponent(Actor* owner) : Component(owner) {
	dampConstant = 2.0f * pow(springConstant, 0.5f);
	cameraVel = Vector3(0.0f, 0.0f, 0.0f);
}

void CameraComponent::Update(float deltaTime) {
	Vector3 idealPosition = ideal();
	Vector3 Displacement = cameraPos - idealPosition;
	Vector3 SpringAccel = (-1.0f * springConstant * Displacement) - (dampConstant * cameraVel);
	cameraVel += SpringAccel * deltaTime;
	cameraPos += cameraVel * deltaTime;
	Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward() * targetOffset);
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ);
	mOwner->mGame->GetRenderer()->SetViewMatrix(view);
}

Vector3 CameraComponent::ideal() {
	Vector3 cameraPos = mOwner->GetPosition() - (horizontalDist * mOwner->GetForward()) + (Vector3::UnitZ * 70.0f);
	return cameraPos;
}

void CameraComponent::SnapToIdeal() {
	cameraPos = ideal();
	Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward() * targetOffset);
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ);
	mOwner->mGame->GetRenderer()->SetViewMatrix(view);
}
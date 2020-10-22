#include "CameraComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Math.h"

CameraComponent::CameraComponent(Actor* owner) : Component(owner) {
}

void CameraComponent::Update(float deltaTime) {
	mPitchAngle += mPitchSpeed * deltaTime;
	if (mPitchAngle > Math::Pi / 4.0f) {
		mPitchAngle = Math::Pi / 4.0f;
	}
	if (mPitchAngle < Math::Pi / -4.0f) {
		mPitchAngle = Math::Pi / -4.0f;
	}


	Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);

	Matrix4 combined = pitch * yaw;
	Vector3 transformed = Vector3::TransformWithPerspDiv(Vector3(1, 0, 0), combined);

	Vector3 targetPos = mOwner->GetPosition() + (transformed * 2.0f);
	Matrix4 view = Matrix4::CreateLookAt(mOwner->GetPosition(), targetPos, Vector3::UnitZ);
	mOwner->mGame->GetRenderer()->SetViewMatrix(view);

}



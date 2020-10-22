#include "CameraComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Math.h"
#include "PlayerMove.h"

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

	

	Vector3 zPos = Vector3::UnitZ;
	Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);


	Matrix4 combined = pitch * yaw;
	Vector3 transformed = Vector3::TransformWithPerspDiv(Vector3(1, 0, 0), combined);

	Vector3 targetPos = mOwner->GetPosition() + (transformed * 2.0f);


	PlayerMove* playMove = mOwner->mGame->playerMove;
	Vector3 rollTransform = Vector3::UnitZ;

	if (playMove->mCurrentState == WallRun) {
		Vector3 targetVector;
		if (playMove->mCurrentSide == CollSide::SideMaxX || playMove->mCurrentSide == CollSide::SideMinX) {
			if (playMove->mCurrentSide == CollSide::SideMaxX) {
				currentRoll += 2.0f * deltaTime;
			}
			else {
				currentRoll += -2.0f * deltaTime;
			}

			if (currentRoll > Math::Pi / 6.0f) {
				currentRoll = Math::Pi / 6.0f;
			}
			if (currentRoll < Math::Pi / -6.0f) {
				currentRoll = Math::Pi / -6.0f;
			}

			Matrix4 roll = Matrix4::CreateRotationY(currentRoll);
			rollTransform = Vector3::TransformWithPerspDiv(Vector3(0, 0, 1), roll);
			Matrix4 view = Matrix4::CreateLookAt(mOwner->GetPosition(), targetPos, rollTransform);
			mOwner->mGame->GetRenderer()->SetViewMatrix(view);
		}
		if (playMove->mCurrentSide == CollSide::SideMaxY || playMove->mCurrentSide == CollSide::SideMinY) {			
			if (playMove->mCurrentSide == CollSide::SideMaxY) {
				currentRoll += -2.0f * deltaTime;
			}
			else {
				currentRoll += 2.0f * deltaTime;
			}

			if (currentRoll > Math::Pi / 6.0f) {
				currentRoll = Math::Pi / 6.0f;
			}
			if (currentRoll < Math::Pi / -6.0f) {
				currentRoll = Math::Pi / -6.0f;
			}

			Matrix4 roll = Matrix4::CreateRotationX(currentRoll);
			rollTransform = Vector3::TransformWithPerspDiv(Vector3(0, 0, 1), roll);
			Matrix4 view = Matrix4::CreateLookAt(mOwner->GetPosition(), targetPos, rollTransform);
			mOwner->mGame->GetRenderer()->SetViewMatrix(view);
		}
	}
	else if (playMove->mCurrentState == Falling) {
		if (currentRoll < 0.0f) {
			currentRoll += 2.0f * deltaTime;
		}
		if (currentRoll > 0.0f) {
			currentRoll += -2.0f * deltaTime;
		}
		if (playMove->mCurrentSide == CollSide::SideMaxX || playMove->mCurrentSide == CollSide::SideMinX) {
			Matrix4 roll = Matrix4::CreateRotationY(currentRoll);
			rollTransform = Vector3::TransformWithPerspDiv(Vector3(0, 0, 1), roll);
			Matrix4 view = Matrix4::CreateLookAt(mOwner->GetPosition(), targetPos, rollTransform);
			mOwner->mGame->GetRenderer()->SetViewMatrix(view);
		}

		if (playMove->mCurrentSide == CollSide::SideMaxY || playMove->mCurrentSide == CollSide::SideMinY) {
			Matrix4 roll = Matrix4::CreateRotationX(currentRoll);
			rollTransform = Vector3::TransformWithPerspDiv(Vector3(0, 0, 1), roll);
			Matrix4 view = Matrix4::CreateLookAt(mOwner->GetPosition(), targetPos, rollTransform);
			mOwner->mGame->GetRenderer()->SetViewMatrix(view);
		}
	}

	
	Matrix4 view = Matrix4::CreateLookAt(mOwner->GetPosition(), targetPos, rollTransform);
	mOwner->mGame->GetRenderer()->SetViewMatrix(view);
}

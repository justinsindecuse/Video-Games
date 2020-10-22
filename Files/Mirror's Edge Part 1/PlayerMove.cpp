#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include <string>
#include "CameraComponent.h"
#include "Random.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

PlayerMove::PlayerMove(Actor* owner) : MoveComponent(owner) {
	mOwner = owner;
	ChangeState(Falling);
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_W]) {
		AddForce(mOwner->GetForward() * 700.0f);
	}
	if (keyState[SDL_SCANCODE_S]) {
		AddForce(mOwner->GetForward() * -700.0f);
	}

	if (keyState[SDL_SCANCODE_D]) {
		AddForce(mOwner->GetRight() * 700.0f);
	}
	if (keyState[SDL_SCANCODE_A]) {
		AddForce(mOwner->GetRight() * -700.0f);
	}

	if ((keyState[SDL_SCANCODE_SPACE] && !pressed) && mCurrentState == OnGround) {
		AddForce(mJumpForce);
		pressed = true;
		ChangeState(Jump);
	}
	if (!keyState[SDL_SCANCODE_SPACE]){
		pressed = false;
	}

	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	float angSpeed = (float)x;
	angSpeed = (angSpeed / 500.0f) * .4f * Math::Pi;
	SetAngularSpeed(angSpeed);
	float pitchSpeed = (float)y;
	pitchSpeed = (pitchSpeed / 500.0f) * 20.0f * Math::Pi;

	mOwner->mGame->playerCam->SetPitchSpeed(pitchSpeed);
}

void PlayerMove::Update(float deltaTime) {
	if (mCurrentState == OnGround) {
		UpdateOnGround(deltaTime);
	}
	else if (mCurrentState == Jump) {
		UpdateJump(deltaTime);
	}
	else {
		UpdateFalling(deltaTime);
	}
}

void PlayerMove::ChangeState(MoveState state) {
	mCurrentState = state;
}

void PlayerMove::UpdateOnGround(float deltaTime) {
	PhysicsUpdate(deltaTime);
	bool falling = true;
	for (unsigned int i = 0; i < mOwner->mGame->objects.size(); i++) {
		if (FixCollision(mOwner->mGame->playerCol, mOwner->mGame->objects[i]->GetComponent<CollisionComponent>()) == CollSide::Top) {
			falling = false;
		}
	}
	if (falling) {
		ChangeState(Falling);
	}
}

void PlayerMove::UpdateJump(float deltaTime) {
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	for (unsigned int i = 0; i < mOwner->mGame->objects.size(); i++) {
		if (FixCollision(mOwner->mGame->playerCol, mOwner->mGame->objects[i]->GetComponent<CollisionComponent>()) == CollSide::Bottom) {
			mVelocity.z = 0.0f;
		}
	}
	if (mVelocity.z <= 0.0f) {
		ChangeState(Falling);
	}
}

void PlayerMove::UpdateFalling(float deltaTime) {
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	for (unsigned int i = 0; i < mOwner->mGame->objects.size(); i++) {
		if (FixCollision(mOwner->mGame->playerCol, mOwner->mGame->objects[i]->GetComponent<CollisionComponent>()) == CollSide::Top) {
			mVelocity.z = 0.0f;
			ChangeState(OnGround);
		}
	}
}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block) {
	Vector3 offset;
	CollSide temp = self->GetMinOverlap(block, offset);
	if (temp != CollSide::None) {
		mOwner->SetPosition(mOwner->GetPosition() + offset);
	}
	return temp;
}

void PlayerMove::PhysicsUpdate(float deltaTime) {
	mAcceleration = mPendingForces * (1/mMass);
	mVelocity += mAcceleration * deltaTime;
	FixXYVelocity();
	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
	mOwner->SetRotation(mOwner->GetRotation() + GetAngularSpeed());
	mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity() {
	Vector2 temp = Vector2(mVelocity.x, mVelocity.y);
	if (temp.Length() > 400.0f) {
		temp.Normalize();
		temp *= 400.0f;
	}
	if (mCurrentState == OnGround) {
		if (Math::NearZero(mAcceleration.x) || (mAcceleration.x * temp.x) < 0) {
			temp.x *= 0.9f;
		}
		if (Math::NearZero(mAcceleration.y) || (mAcceleration.y * temp.y) < 0) {
			temp.y *= 0.9f;
		}
	}
	mVelocity = Vector3(temp.x, temp.y, mVelocity.z);
}
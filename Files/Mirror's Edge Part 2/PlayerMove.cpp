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
	else if (mCurrentState == Falling){
		UpdateFalling(deltaTime);
	}
	else if(mCurrentState == WallClimb) {
		UpdateWallClimb(deltaTime);
	}
	else {
		UpdateWallRun(deltaTime);
	}
}

void PlayerMove::ChangeState(MoveState state) {
	mCurrentState = state;
}

void PlayerMove::UpdateOnGround(float deltaTime) {
	PhysicsUpdate(deltaTime);
	bool falling = true;
	for (unsigned int i = 0; i < mOwner->mGame->objects.size(); i++) {
		CollisionComponent* gameObject = mOwner->mGame->objects[i]->GetComponent<CollisionComponent>();
		CollSide test = FixCollision(mOwner->mGame->playerCol, gameObject);
		if (test == CollSide::Top) {
			falling = false;
		}
		else if (test != CollSide::Bottom && test != CollSide::None) {
			if (CanWallClimb(test) == true) {
				mWallClimbTimer = 0.0f;
				ChangeState(WallClimb);
				return;
			}
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
		CollisionComponent* gameObject = mOwner->mGame->objects[i]->GetComponent<CollisionComponent>();
		CollSide test = FixCollision(mOwner->mGame->playerCol, gameObject);
		if (test == CollSide::Bottom) {
			mVelocity.z = 0.0f;
		}
		else if (test != CollSide::Bottom && test != CollSide::None) {
			if (CanWallClimb(test) == true) {
				mWallClimbTimer = 0.0f;
				ChangeState(WallClimb);
				return;
			}
			else if (CanWallRun(test) == true) {
				mWallRunTimer = 0.0f;
				ChangeState(WallRun);
				mCurrentSide = test;
				return;
			}
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
		CollisionComponent* gameObject = mOwner->mGame->objects[i]->GetComponent<CollisionComponent>();
		CollSide test = FixCollision(mOwner->mGame->playerCol, gameObject);
		if (test == CollSide::Top) {
			mVelocity.z = 0.0f;
			ChangeState(OnGround);
		}
	}
}

void PlayerMove::UpdateWallClimb(float deltaTime) {
	mWallClimbTimer += deltaTime;
	bool falling = true;
	if (mWallClimbTimer < 0.4f) {
		AddForce(mWallClimbForce);
	}
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	for (unsigned int i = 0; i < mOwner->mGame->objects.size(); i++) {
		CollisionComponent* gameObject = mOwner->mGame->objects[i]->GetComponent<CollisionComponent>();
		CollSide test = FixCollision(mOwner->mGame->playerCol, gameObject);
		if ((test == CollSide::SideMaxX || test == CollSide::SideMaxY) || (test == CollSide::SideMinX || test == CollSide::SideMinY)) {
			falling = false;
		}
	}
	if (falling || mVelocity.z <= 0.0f) {
		ChangeState(Falling);
	}
}

void PlayerMove::UpdateWallRun(float deltaTime) {
	mWallRunTimer += deltaTime;
	if (mWallRunTimer < 0.6f) {
		AddForce(mWallRunForce);
	}
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	for (unsigned int i = 0; i < mOwner->mGame->objects.size(); i++) {
		CollisionComponent* gameObject = mOwner->mGame->objects[i]->GetComponent<CollisionComponent>();
		CollSide test = FixCollision(mOwner->mGame->playerCol, gameObject);
	}
	if (mVelocity.z <= 0.0f) {
		ChangeState(Falling);
	}
}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block) {
	Vector3 offset;
	CollSide temp = self->GetMinOverlap(block, offset);
	if (temp != CollSide::None) {
		mOwner->SetPosition(mOwner->GetPosition() + offset);
	}

	Vector3 normalForce;
	if (temp == CollSide::SideMaxX) {
		normalForce = Vector3(700.0f, 0.0f, 0.0f);
	}
	else if (temp == CollSide::SideMinX) {
		normalForce = Vector3(-700.0f, 0.0f, 0.0f);
	}
	else if (temp == CollSide::SideMaxY) {
		normalForce = Vector3(0.0f, 700.0f, 0.0f);
	}
	else if (temp == CollSide::SideMinY){
		normalForce = Vector3(0.0f, -700.0f, 0.0f);
	}
	AddForce(normalForce);

	return temp;
}

void PlayerMove::PhysicsUpdate(float deltaTime) {
	mAcceleration = mPendingForces * (1 / mMass);
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
	if (mCurrentState == OnGround || mCurrentState == WallClimb) {
		if (Math::NearZero(mAcceleration.x) || (mAcceleration.x * temp.x) < 0) {
			temp.x *= 0.9f;
		}
		if (Math::NearZero(mAcceleration.y) || (mAcceleration.y * temp.y) < 0) {
			temp.y *= 0.9f;
		}
	}
	mVelocity = Vector3(temp.x, temp.y, mVelocity.z);
}

bool PlayerMove::CanWallClimb(CollSide side) {
	Vector3 targetVector;
	bool facingSame = false;
	bool xyFaceSide = false;
	bool speedRequirement = false;
	if (side == CollSide::SideMaxX) {
		targetVector = Vector3(-1.0f, 0.0f, 0.0f);
	}
	else if (side == CollSide::SideMinX) {
		targetVector = Vector3(1.0f, 0.0f, 0.0f);
	}
	else if (side == CollSide::SideMaxY) {
		targetVector = Vector3(0.0f, -1.0f, 0.0f);
	}
	else {
		targetVector = Vector3(0.0f, 1.0f, 0.0f);
	}

	float dotProduct = Vector3::Dot(mOwner->GetForward(), targetVector);
	if (dotProduct >= 0.80) {
		facingSame = true;
	}

	Vector3 testVel = mVelocity;
	testVel.Normalize();
	dotProduct = Vector3::Dot(testVel, targetVector);
	if (dotProduct > 0.80f) {
		xyFaceSide = true;
	}

	Vector2 speed = Vector2(mVelocity.x, mVelocity.y);
	if (speed.Length() >= 350.0f) {
		speedRequirement = true;
	}

	if ((facingSame && xyFaceSide) && speedRequirement){
		return true;
	}
	return false;
}

bool PlayerMove::CanWallRun(CollSide side) {
	Vector3 targetVector;
	bool facingSame = false;
	bool xyFaceSide = false;
	bool speedRequirement = false;
	if (side == CollSide::SideMaxX) {
		targetVector = Vector3(-1.0f, 0.0f, 0.0f);
	}
	else if (side == CollSide::SideMinX) {
		targetVector = Vector3(1.0f, 0.0f, 0.0f);
	}
	else if (side == CollSide::SideMaxY) {
		targetVector = Vector3(0.0f, -1.0f, 0.0f);
	}
	else {
		targetVector = Vector3(0.0f, 1.0f, 0.0f);
	}

	float dotProduct = Vector3::Dot(mOwner->GetForward(), targetVector);
	if (dotProduct < 0.80f && dotProduct > 0.0f) {
		facingSame = true;
	}

	Vector3 testVel = mVelocity;
	testVel.Normalize();
	Vector2 holder = Vector2(testVel.x, testVel.y);
	Vector2 holder2 = Vector2(mOwner->GetForward().x, mOwner->GetForward().y);
	dotProduct = Vector2::Dot(holder, holder2);
	if (dotProduct > 0.10f) {
		xyFaceSide = true;
	}

	Vector2 speed = Vector2(mVelocity.x, mVelocity.y);
	if (speed.Length() >= 350.0f) {
		speedRequirement = true;
	}

	if ((facingSame && xyFaceSide) && speedRequirement) {
		return true;
	}
	return false;
}
#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"
#include <vector>

enum MoveState {
	OnGround,
	Jump,
	Falling,
	WallClimb,
	WallRun
};

class PlayerMove : public MoveComponent {
public:
	PlayerMove(class Actor* owner);
	Vector3 velocity;
	Actor* mOwner;
	MoveState mCurrentState;
	void ChangeState(MoveState state);
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mPendingForces;
	float mMass = 1.0f;
	Vector3 mGravity = Vector3(0.0f, 0.0f, -980.0f);
	Vector3 mJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
	Vector3 mWallClimbForce = Vector3(0.0f, 0.0f, 1800.0f);
	Vector3 mWallRunForce = Vector3(0.0f, 0.0f, 1200.0f);

	bool pressed = false;
	float mWallClimbTimer = 0.0f;
	float mWallRunTimer = 0.0f;
	CollSide mCurrentSide = CollSide::None;

	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force) { mPendingForces += force; }
	void FixXYVelocity();
protected:
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);
	void UpdateWallClimb(float deltaTime);
	void UpdateWallRun(float deltaTime);
	bool CanWallClimb(CollSide side);
	bool CanWallRun(CollSide side);
	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
private:
	void Update(float deltaTime);
	void ProcessInput(const Uint8* keyState);
};
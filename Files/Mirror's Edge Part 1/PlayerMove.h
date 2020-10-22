#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"
#include <vector>

enum MoveState {
	OnGround,
	Jump,
	Falling
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
	bool pressed = false;

	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force) { mPendingForces += force; }
	void FixXYVelocity();
protected:
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);
	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
private:
	void Update(float deltaTime);
	void ProcessInput(const Uint8* keyState);
};
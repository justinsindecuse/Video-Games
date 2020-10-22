#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"

class Actor;

class BallMove : public MoveComponent {
public:
	BallMove(Actor* owner);
	~BallMove();
	void Update(float deltaTime);

private:
	Vector2 Velocity;
	Vector2 LeftNormal;
	Vector2 MidNormal;
	Vector2 RightNormal;
	Vector2 initial;
	float wallLeft;
	float wallRight;
	float wallTop;
	Vector2 initialV;
	CollisionComponent* ballCol;
	Actor* mOwner;
	float amount;
};
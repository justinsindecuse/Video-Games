#pragma once
#include "MoveComponent.h"

class Actor;
class CollisionComponent;

class PlayerMove : public MoveComponent {
public:
	PlayerMove(Actor* owner);
	~PlayerMove();
private:
	Actor* mOwner;
	void ProcessInput(const Uint8* keyState);
	void Update(float deltaTime);
	float mYSpeed;
	bool mSpacePressed;
	bool mInAir;
	void determineAnim();
	bool dead;
	CollisionComponent* playCol;
};
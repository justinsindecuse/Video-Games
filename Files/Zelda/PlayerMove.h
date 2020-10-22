#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include <string>

class Actor;
class AnimatedSprite;
class CollisionComponent;

enum face {
	up,
	down,
	right,
	left
};

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
	Vector2 direction;
	face facing;
	std::string DirectFace;
	AnimatedSprite* playSprite;
};
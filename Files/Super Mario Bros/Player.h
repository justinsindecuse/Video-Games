#pragma once
#include "Actor.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

class Player : public Actor {
public:
	Player(Game* game);
	~Player();

private:
	AnimatedSprite* playerTexture;
	CollisionComponent* playerCol;
	PlayerMove* playMove;
};
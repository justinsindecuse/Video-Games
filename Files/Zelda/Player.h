#pragma once
#include "Actor.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"


class Player : public Actor {
public:
	Player(Game* game);
	~Player();
	AnimatedSprite* playerTexture;

private:
	CollisionComponent* playerCol;
	PlayerMove* playMove;
};
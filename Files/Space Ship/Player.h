#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

class Game;

class Player : public Actor {
public:
	Player(Game* game);
	MeshComponent* mc;
	CollisionComponent* playerCol;
	PlayerMove* pm;
private:

};
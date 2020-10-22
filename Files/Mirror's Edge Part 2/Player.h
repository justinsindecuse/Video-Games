#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "CameraComponent.h"

class Game;

class Player : public Actor {
public:
	Player(Game* mGame, Actor* parent);
	PlayerMove* pm;
	CameraComponent* cm;
	CollisionComponent* playColl;
private:

};
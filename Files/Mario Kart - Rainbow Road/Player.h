#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "PlayerUI.h"

class Game;

class Player : public Actor {
public:
	Player(Game* game);
	MeshComponent* mc;
	PlayerMove* pm;
	CameraComponent* cm;
	PlayerUI* pu;
private:

};
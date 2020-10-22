#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "EnemyMove.h"
#include <vector>

class Enemy : public Actor {
public:
	Enemy(Game* game);
private:
	MeshComponent* mc;
	EnemyMove* em;
	
};
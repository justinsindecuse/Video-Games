#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include <vector>

class Block : public Actor {
public:
	Block(Game* mGame);
	~Block();
	MeshComponent* meshBlock;
	CollisionComponent* colBlock;
private:

};
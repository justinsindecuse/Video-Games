#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"

class Block : public Actor {
public:
	Block(class Game* game);
	~Block();
	MeshComponent* mc;
	CollisionComponent* col;
	bool exploding = false;
	bool exploded = false;
	void explode();
private:
	void OnUpdate(float deltaTime);
	
	bool inRange(Vector3 x);
};
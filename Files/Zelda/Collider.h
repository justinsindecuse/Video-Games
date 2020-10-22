#pragma once
#include "Actor.h"

class CollisionComponent;

class Collider : public Actor {
public:
	Collider(Game* game);
	~Collider();
	void setCollision(int width, int height);
	CollisionComponent* col;

private:
	
};
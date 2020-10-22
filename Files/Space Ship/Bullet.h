#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

class Game;

class Bullet : public Actor {
public:
	Bullet(Game* game);
	MeshComponent* mc;
	MoveComponent* bulMov;
	CollisionComponent* bulCol;
	float timer = 0.0f;
private:
	void OnUpdate(float deltaTime);
};
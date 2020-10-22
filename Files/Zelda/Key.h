#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

class Game;

class Key : public Actor {
public:
	Key(Game* game);
	CollisionComponent* keyCol;
private:
	SpriteComponent* keyTexture;
};
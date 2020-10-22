#pragma once
#include "Actor.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"

class Game;

class SecretBlock : public Actor {
public:
	SecretBlock(Game* game);
	~SecretBlock();
	CollisionComponent* sbCol;
	float initial;
private:
	SpriteComponent* sbTexture;

};
#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include <string>

class Game;

class Paddle : public Actor {
public:
	Paddle(Game* game);
	~Paddle();
	Game* thisGame;
	CollisionComponent* PaddleCol;

private:
	SpriteComponent* PaddleTexture;	
	MoveComponent* PaddleMove;
	float limitLeft;
	float limitRight;
	void OnProcessInput(const Uint8* keyState);
	void OnUpdate(float deltaTime);
};
#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "BallMove.h"
#include "CollisionComponent.h"

class Game;

class Ball : public Actor {
public:
	Ball(Game* game);	
	~Ball();
	Game* thisGame;
	CollisionComponent* BallCol;

private:
	SpriteComponent* BallTexture;
	BallMove* BallMove1;
	
	
};
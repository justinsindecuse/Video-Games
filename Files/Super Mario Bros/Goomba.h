#pragma once
#include "Actor.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "GoombaMove.h"

class Game;

class Goomba : public Actor {
public:
	Goomba(Game*);
	~Goomba();
	CollisionComponent* GoombaCol;
	
	AnimatedSprite* GoombaTexture;
private:
	
	GoombaMove* GoomMove;
	
};

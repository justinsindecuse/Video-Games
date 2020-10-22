#pragma once

#include "Actor.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include <string>

class Game;

class Block : public Actor 
{
public:
	Block(Game* game);
	~Block();
	void changeTexture(std::string fileName);
	CollisionComponent* blockCol;

private:
	SpriteComponent* blockTexture;
	Game* thisGame;
	
	int placement;
};
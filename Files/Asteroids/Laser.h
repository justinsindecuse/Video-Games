#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

class Laser : public Actor
{
public:
	Laser(class Game* game);
	~Laser();
	void OnUpdate(float deltaTime);


private:
	SpriteComponent LaserSprite;
	MoveComponent LaserMovement;
	SDL_Texture* stasis;
	Game* gameReal;
	float duration;
};
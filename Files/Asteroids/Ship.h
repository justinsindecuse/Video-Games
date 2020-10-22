#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Laser.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);
	~Ship();
	void OnProcessInput(const Uint8* keyState);
	float duration;


private:
	SpriteComponent shipSprite;
	MoveComponent shipMovement;
	SDL_Texture* stasis;
	SDL_Texture* thrust;
	Game* gameReal;
	void OnUpdate(float deltaTime);
	int screenw;
	int screenh;
};
#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	~Asteroid();
	void OnUpdate(float deltaTime);


private:
	SpriteComponent asteroidSprite;
	MoveComponent asteroidMovement;
	SDL_Texture* stasis;
	int screenw;
	int screenh;
};
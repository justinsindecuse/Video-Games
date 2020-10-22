#pragma once
#include "Actor.h"
#include <string>

class Game;
class CollisionComponent;
class SpriteComponent;

class Door : public Actor {
public:
	Door(Game* game);
	~Door();
	void DoorSetup(std::string Destination, std::string Direction, std::string State);
	CollisionComponent* DoorCol;
	Vector2 mResult;
	std::string mDirection;
	std::string mDestination;
	std::string mState;
	std::string destDirection;

private:
	
	SpriteComponent* DoorTexture;
};
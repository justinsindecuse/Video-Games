#pragma once
#include "Actor.h"
#include "MeshComponent.h"

class Game;

class SideBlock : public Actor {
public:
	SideBlock(Game* game);
	MeshComponent* mc;
private:
	void OnUpdate(float deltaTime);
};
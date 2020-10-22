#pragma once
#include "Actor.h"
#include "LaserComponent.h"
#include "MeshComponent.h"

class LaserMine : public Actor {
public:
	LaserMine(class Game* game, Actor* parent);
	MeshComponent* mc;
	LaserComponent* lc;
private:

};
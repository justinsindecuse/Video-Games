#pragma once
#include "Actor.h"

class Spawner : public Actor {
public:
	Spawner(Game* game);
	~Spawner();

private:
	void OnUpdate(float deltaTime);

};
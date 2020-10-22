#pragma once
#include "Actor.h"
#include <string>

class Spawner : public Actor {
public:
	Spawner(Game* game);
	~Spawner();
	std::string type;
private:
	
};
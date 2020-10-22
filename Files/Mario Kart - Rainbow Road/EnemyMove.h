#pragma once
#include "VehicleMove.h"
#include <vector>

class EnemyMove : public VehicleMove {
public:
	EnemyMove(Actor* owner);
	void Update(float deltaTime);
	int nextNode = 1;
private:
	std::vector<Vector3> pathNodes;
	float distance(Vector3 first, Vector3 second);
};
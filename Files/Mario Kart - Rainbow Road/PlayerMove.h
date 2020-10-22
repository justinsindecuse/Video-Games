#pragma once
#include "MoveComponent.h"
#include "VehicleMove.h"
#include "Math.h"
#include "CollisionComponent.h"
#include <vector>

class PlayerMove : public VehicleMove {
public:
	PlayerMove(class Actor* owner);
	Vector3 velocity;
	Actor* mOwner;
	void OnLapChange(int newLap);
private:
	void Update(float deltaTime);
	void ProcessInput(const Uint8* keyState);
};
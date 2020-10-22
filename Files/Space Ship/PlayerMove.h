#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"
#include <vector>

class PlayerMove : public MoveComponent {
public:
	PlayerMove(class Actor* owner);
	Vector3 velocity;
	Actor* mOwner;
	float multiplier = 1.0f;
private:
	void Update(float deltaTime);
	void ProcessInput(const Uint8* keyState);
	float wallX;
	int index;
	std::vector<int> colors;
	int txtNum = 1;
	bool exceeds = false;
	bool mSpacePressed = false;
	CollisionComponent* test;
	int channel;
	float tracker = 0.0f;
};
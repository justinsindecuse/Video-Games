#pragma once
#include "MoveComponent.h"

class CollisionComponent;

class GoombaMove : public MoveComponent {
public:
	GoombaMove(Actor*);
	~GoombaMove();
private:
	float mYSpeed;
	void Update(float deltaTime);
	float duration;
	CollisionComponent* goomCol;
};
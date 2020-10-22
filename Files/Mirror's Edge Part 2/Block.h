#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include <vector>

class Block : public Actor {
public:
	Block(Game* mGame, Actor* parent);
	~Block();
	MeshComponent* meshBlock;
	CollisionComponent* colBlock;
	void OnUpdate(float deltaTime);
	bool GetRotate() { return rotates; }
	void SetRotate(bool x) { rotates = x; }
private:
	bool rotates;
};
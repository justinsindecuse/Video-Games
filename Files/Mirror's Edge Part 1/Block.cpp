#include "Block.h"
#include "Game.h"
#include "Renderer.h"

Block::Block(Game* game) : Actor(game) {
	meshBlock = new MeshComponent(this);
	meshBlock->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	colBlock = new CollisionComponent(this);
	SetScale(64.0f);
	colBlock->SetSize(1.0f, 1.0f, 1.0f);
	mGame->objects.push_back(this);
}

Block::~Block() {
	std::vector<class Actor*>::iterator it_vector;
	it_vector = std::find(mGame->objects.begin(), mGame->objects.end(), this);
	if (it_vector != mGame->objects.end()) {
		mGame->objects.erase(it_vector);
	}
	else {
		return;
	}
}
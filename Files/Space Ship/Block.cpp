#include "Block.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

Block::Block(Game *game) : Actor(game) {
	SetScale(25.0f);
	mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	if (!exploding) {
		mc->SetTextureIndex(2);
	}
	else {
		mc->SetTextureIndex(4);
	}
	col = new CollisionComponent(this);
	col->SetSize(1, 1, 1);
}

Block::~Block() {
	mGame->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime) {
	if (exploding) {
		mc->SetTextureIndex(4);
	}
	if (mPosition.x < mGame->mPlayer->GetPosition().x - 500) {
		SetState(ActorState::Destroy);
	}
}

void Block::explode() {
	exploded = true;
	for (unsigned int i = 0; i < mGame->blocks.size(); i++) {
		if (inRange(mGame->blocks[i]->GetPosition())) {
			if (!mGame->blocks[i]->exploding) {
				mGame->blocks[i]->SetState(ActorState::Destroy);
			}
			else if(!mGame->blocks[i]->exploded){
				mGame->blocks[i]->explode();
			}
		}
	}
	SetState(ActorState::Destroy);
}

bool Block::inRange(Vector3 other) {
	Vector3 temp = GetPosition() - other;
	if (pow(pow(temp.y, 2) + pow(temp.z, 2), 0.5) < 50.0f && GetPosition().x == other.x) {
		return true;
	}
	else {
		return false;
	}
}
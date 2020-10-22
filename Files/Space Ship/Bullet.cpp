#include "Bullet.h"
#include "Game.h"
#include "Block.h"
#include "Renderer.h" 
#include "Player.h"
#include "PlayerMove.h"

Bullet::Bullet(Game* game) : Actor(game) {
	this->SetScale(0.5f);
	mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
	bulMov = new MoveComponent(this);
	float forward = mGame->mPlayer->GetComponent<PlayerMove>()->multiplier * 400.0f;
	bulMov->SetForwardSpeed(forward + 500.0f);
	bulCol = new CollisionComponent(this);
	bulCol->SetSize(10, 10, 10);
}

void Bullet::OnUpdate(float deltaTime) {
	timer += deltaTime;
	if (timer > 1.0f) {
		SetState(ActorState::Destroy);
	}
	for (unsigned int i = 0; i < mGame->blocks.size(); i++) {
		if (bulCol->Intersect(mGame->blocks[i]->col)){
			if (!mGame->blocks[i]->exploding) {
				SetState(ActorState::Destroy);
			}
			else {
				Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BlockExplode.wav"), 0, -1);
				mGame->blocks[i]->explode();
				SetState(ActorState::Destroy);
			}
		}
	}
}
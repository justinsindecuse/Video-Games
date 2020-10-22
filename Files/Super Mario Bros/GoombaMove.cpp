#include "GoombaMove.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Block.h"
#include "Goomba.h"

GoombaMove::GoombaMove(Actor* owner) : MoveComponent(owner) {
	SetForwardSpeed(-150.0f);
	mYSpeed = 0.0f;
	duration = 0.25f;
	goomCol = mOwner->GetComponent<CollisionComponent>();
}

GoombaMove::~GoombaMove() {

}

void GoombaMove::Update(float deltaTime) {
	if (!mOwner->stomped) {
		float xPos = mOwner->GetPosition().x + (deltaTime * GetForwardSpeed());
		mOwner->SetPosition(Vector2(xPos, mOwner->GetPosition().y + (deltaTime * mYSpeed)));
	}

	Vector2 offset;
	CollSide side = CollSide::None;
	//First Pass
	for (unsigned int i = 0; i < mOwner->mGame->blocks.size(); i++) {
		side = goomCol->GetMinOverlap(mOwner->mGame->blocks[i]->blockCol, offset);
		if (side == CollSide::Top) {
			mYSpeed = 0.0f;
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}			
	}
	//Second Pass
	for (unsigned int i = 0; i < mOwner->mGame->blocks.size(); i++) {
		side = goomCol->GetMinOverlap(mOwner->mGame->blocks[i]->blockCol, offset);
		if (side == CollSide::Right || side == CollSide::Left) {
			if (!(mOwner->mGame->blocks[i]->blockCol->GetMax().y > goomCol->GetMax().y + 5.0f)) {
				SetForwardSpeed(-1.0f * GetForwardSpeed());
				mOwner->SetPosition(mOwner->GetPosition() + offset);
			}
		}
	}
	//Goomba Bouncing off one another
	for (unsigned int i = 0; i < mOwner->mGame->enemies.size(); i++) {
		CollisionComponent* holder = mOwner->mGame->enemies[i]->GoombaCol;
		side = goomCol->GetMinOverlap(holder, offset);
		if (side == CollSide::Right || side == CollSide::Left) {
			if (!(holder->GetMax().y > goomCol->GetMax().y + 5.0f)) {
				SetForwardSpeed(-1.0f * GetForwardSpeed());
				mOwner->SetPosition(mOwner->GetPosition() + offset);
			}
		}
	}
	mYSpeed += 2000.0f * deltaTime;

	if (mOwner->GetPosition().y >= 500.0f) {
		mOwner->SetState(ActorState::Destroy);
	}

	if (mOwner->stomped) {
		duration -= deltaTime;
		if (duration <= 0.0f) {
			mOwner->SetState(ActorState::Destroy);
		}
	}
}
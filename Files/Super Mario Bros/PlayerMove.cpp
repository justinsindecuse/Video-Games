#include "PlayerMove.h"
#include "Game.h"
#include "Actor.h"
#include "Block.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Goomba.h"

PlayerMove::PlayerMove(Actor* owner) : MoveComponent(owner) {
	mOwner = owner;
	mYSpeed = 0.0f;
	mSpacePressed = false;
	mInAir = false;
	dead = false;
	playCol = mOwner->GetComponent<CollisionComponent>();
	Mix_PlayChannel(10, mOwner->mGame->GetSound("Assets/Sounds/ShipLoop.wav"), -1);
}

PlayerMove::~PlayerMove() {

}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_RIGHT]) {
		SetForwardSpeed(200.0f);
	}
	else if (keyState[SDL_SCANCODE_LEFT]) {
		SetForwardSpeed(-200.0f);
	}
	if ((!keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT]) || (keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_LEFT])) {
		SetForwardSpeed(0.0f);
	}
	if ((keyState[SDL_SCANCODE_SPACE] && mSpacePressed == false) && mInAir == false) {
		mSpacePressed = true;
		mYSpeed = -700.0f;
		mInAir = true;
		Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/Jump.wav"), 0);
	}
	if (!keyState[SDL_SCANCODE_SPACE]) {
		mSpacePressed = false;
	}

}

void PlayerMove::Update(float deltaTime) {
	bool touching = false;
	//X position
	float xPos = mOwner->GetPosition().x + (deltaTime * GetForwardSpeed());
	//Change to x and y position
	mOwner->SetPosition(Vector2(xPos, mOwner->GetPosition().y + (deltaTime * mYSpeed)));
	//So mario doesn't fall through screen
	if (mOwner->GetPosition().y >= 490.0f) {
		dead = true;
		mOwner->SetState(ActorState::Paused);
		
	}
	if (mOwner->GetPosition().x < (mOwner->mGame->cameraPos.x)) {
		mOwner->SetPosition(Vector2(mOwner->mGame->cameraPos.x, mOwner->GetPosition().y));
	}
	//Mario collisions
	Vector2 offset;
	CollSide side = CollSide::None;
	for (unsigned int i = 0; i < mOwner->mGame->blocks.size(); i++) {
		side = playCol->GetMinOverlap(mOwner->mGame->blocks[i]->blockCol, offset);
		if (side != CollSide::None) {
			touching = true;
			if (side == CollSide::Top && mYSpeed > 0.0f) {
				mYSpeed = 0.0f;
				mInAir = false;
			}
			else if (side == CollSide::Bottom) {
				mYSpeed = 0.0f;
				Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/Bump.wav"), 0);
			}
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}
	//Hitting a goomba
	for (unsigned int i = 0; i < mOwner->mGame->enemies.size(); i++) {
		side = playCol->GetMinOverlap(mOwner->mGame->enemies[i]->GoombaCol, offset);
		if (side != CollSide::None) {
			if (!mOwner->mGame->enemies[i]->stomped) {
				if (side == CollSide::Top || ((side == CollSide::Right || side == CollSide::Left) && mInAir)) {
					mOwner->mGame->enemies[i]->stomped = true;
					mYSpeed = -350.0f;
					mInAir = true;
					mOwner->mGame->enemies[i]->GoombaTexture->SetAnimation("dead");
					Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/Stomp.wav"), 0);
				}
				else {
					dead = true;
					mOwner->SetState(ActorState::Paused);
				}
			}
		}
	}
	if (!touching) {
		mInAir = true;
	}
	mYSpeed += 2000.0f * deltaTime;

	if (mOwner->GetPosition().x - 300.0f > mOwner->mGame->cameraPos.x) {
		mOwner->mGame->cameraPos.x = mOwner->GetPosition().x - 300.0f;
	}

	if (mOwner->GetPosition().x > 6368.0f) {
		Mix_HaltChannel(mOwner->mGame->channel);
		Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/StageClear.wav"), 0);
		mOwner->SetState(ActorState::Paused);
	}

	determineAnim();
}

void PlayerMove::determineAnim() {
	AnimatedSprite* temp = mOwner->GetComponent<AnimatedSprite>();
	if (!mInAir) {
		if (GetForwardSpeed() > 0.0f) {
			temp->SetAnimation("rr");
		}
		else if (GetForwardSpeed() < 0.0f) {
			temp->SetAnimation("rl");
		}
		else {
			temp->SetAnimation("idle");
		}
	}
	else {
		if (GetForwardSpeed() > 0.0f) {
			temp->SetAnimation("jr");
		}
		else if (GetForwardSpeed() < 0.0f) {
			temp->SetAnimation("jl");
		}
		else {
			if (temp->GetAnimName() == "rr" || temp->GetAnimName() == "jr" || temp->GetAnimName() == "idle") {
				temp->SetAnimation("jr");
			}
			else {
				temp->SetAnimation("jl");
			}
		}
	}
	if (dead) {
		temp->SetAnimation("dead");
		Mix_HaltChannel(mOwner->mGame->channel);
		Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/Dead.wav"), 0);
	}
}
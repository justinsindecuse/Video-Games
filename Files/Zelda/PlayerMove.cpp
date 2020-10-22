#include "PlayerMove.h"
#include "Game.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Collider.h"
#include "Door.h"
#include "SecretBlock.h"
#include <iostream>

PlayerMove::PlayerMove(Actor* owner) : MoveComponent(owner) {
	mOwner = owner;
	mYSpeed = 0.0f;
	dead = false;
	playCol = mOwner->GetComponent<CollisionComponent>();
	playSprite = mOwner->GetComponent<AnimatedSprite>();
}

PlayerMove::~PlayerMove() {

}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_DOWN]) {
		SetForwardSpeed(130.0f);
		playSprite->SetIsPaused(false);
	}
	else{
		SetForwardSpeed(0.0f);
		playSprite->SetIsPaused(true);
	}
	if (keyState[SDL_SCANCODE_RIGHT]) {
		direction = Vector2(1, 0);
		facing = right;
		DirectFace = "Right";
	}
	else if (keyState[SDL_SCANCODE_LEFT]) {
		direction = Vector2(-1, 0);
		facing = left;
		DirectFace = "Left";
	}
	else if (keyState[SDL_SCANCODE_UP]) {
		direction = Vector2(0, -1);
		facing = up;
		DirectFace = "Up";
	}
	else if (keyState[SDL_SCANCODE_DOWN]) {
		direction = Vector2(0, 1);
		facing = down;
		DirectFace = "Down";
	}

}

void PlayerMove::Update(float deltaTime) {
	Vector2 movement = direction * GetForwardSpeed() * deltaTime;
	mOwner->SetPosition(Vector2(movement + mOwner->GetPosition()));

	if (mOwner->GetPosition().x - 300.0f != mOwner->mGame->cameraPos.x) {
		mOwner->mGame->cameraPos.x = mOwner->GetPosition().x - 300.0f;
	}

	if (mOwner->GetPosition().y - 224.0f != mOwner->mGame->cameraPos.y) {
		mOwner->mGame->cameraPos.y = mOwner->GetPosition().y - 224.0f;
	}

	Vector2 offset;
	CollSide side = CollSide::None;
	//Secret Block Collisions
	for (unsigned int i = 0; i < mOwner->mGame->blocks.size(); i++) {
		side = playCol->GetMinOverlap(mOwner->mGame->blocks[i]->sbCol, offset);
		if ((side == CollSide::Bottom && DirectFace == "Up") && mOwner->mGame->blocks[i]->GetPosition().y > mOwner->mGame->blocks[i]->initial - 32) {
			SetForwardSpeed(65.0f);
			mOwner->mGame->blocks[i]->SetPosition(mOwner->mGame->blocks[i]->GetPosition() - offset);
		}
		else if (side != CollSide::None) {
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			if (mOwner->mGame->blocks[i]->GetPosition().y <= mOwner->mGame->blocks[i]->initial - 32) {
				std::vector<Door*> holder = mOwner->mGame->doorsPerRoom.find(mOwner->mGame->currentRoom)->second;
				for (unsigned int j = 0; j < holder.size(); j++) {
					if (holder[j]->mState == "Closed") {
						holder[j]->mState = "Open";
					}
				}
			}
		}
	}
	//Door Collisions
	std::vector<Door*> holder = mOwner->mGame->doorsPerRoom.find(mOwner->mGame->currentRoom)->second;
	for (unsigned int i = 0; i < holder.size(); i++) {
		side = playCol->GetMinOverlap(holder[i]->DoorCol, offset);
		if ((side != CollSide::None && holder[i]->mDirection == DirectFace) && holder[i]->mState == "Open") {
			std::vector<Door*> destDoors = mOwner->mGame->doorsPerRoom.find(holder[i]->mDestination)->second;
			unsigned int x;
			for (x = 0; x < destDoors.size(); x++) {
				if (destDoors[x]->mDirection == holder[i]->destDirection) {
					break;
				}
			}
			mOwner->SetPosition(destDoors[x]->mResult);
			mOwner->mGame->currentRoom = holder[i]->mDestination;
			mOwner->mGame->activateSpawns();
		}
	}
	//Wall Collisions
	for (unsigned int i = 0; i < mOwner->mGame->walls.size(); i++) {
		side = playCol->GetMinOverlap(mOwner->mGame->walls[i]->col, offset);
		if (side != CollSide::None) {
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}

	determineAnim();
}

void PlayerMove::determineAnim() {
	AnimatedSprite* temp = mOwner->GetComponent<AnimatedSprite>();
	if (facing == right) {
		temp->SetAnimation("walkRight");
	}
	if (facing == left) {
		temp->SetAnimation("walkLeft");
	}
	if (facing == up) {
		temp->SetAnimation("walkUp");
	}
	if (facing == down) {
		temp->SetAnimation("walkDown");
	}
}
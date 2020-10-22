#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "SideBlock.h"
#include "Block.h"
#include <iostream>
#include <string>
#include "Random.h"
#include "Bullet.h"

PlayerMove::PlayerMove(Actor* owner) : MoveComponent(owner) {
	velocity = Vector3(0.0f, 0.0f, 0.0f);
	mOwner = owner;
	wallX = 0.0f;
	index = 0;
	test = mOwner->GetComponent<CollisionComponent>();
	colors.push_back(1);
	colors.push_back(2);
	colors.push_back(4);
	colors.push_back(2);
	for (int i = 0; i < 6; i++) {
		SideBlock* first = new SideBlock(mOwner->mGame);
		first->SetPosition(Vector3(wallX, -500.0f, 0.0f));
		first->mc->SetTextureIndex(colors[index]);
		SideBlock* second = new SideBlock(mOwner->mGame);
		second->SetPosition(Vector3(wallX, 500.0f, 0.0f));
		second->mc->SetTextureIndex(colors[index]);
		wallX += 500.0f;
		if (++index > 3) {
			index = 0;
		}
	}
	channel = Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/ShipLoop.wav"), -1);
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	float forward = 400.0f * multiplier;
	float sideway = 300.0f * multiplier;
	if (keyState[SDL_SCANCODE_W]) {
		velocity = Vector3(forward, 0.0f, sideway);
		if (keyState[SDL_SCANCODE_D]) {
			velocity = Vector3(forward, sideway, sideway);
		}
		if (keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, -1 * sideway, sideway);
		}
		if (keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, 0.0f, sideway);
		}
		if (!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, 0.0f, sideway);
		}
	}
	if (keyState[SDL_SCANCODE_S]) {
		velocity = Vector3(forward, 0.0f, -1 * sideway);
		if (keyState[SDL_SCANCODE_D]) {
			velocity = Vector3(forward, sideway, -1 * sideway);
		}
		if (keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, -1 * sideway, -1 * sideway);
		}
		if (keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, 0.0f, -1 * sideway);
		}
		if (!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, 0.0f, -1 * sideway);
		}
	}
	if (keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]) {
		velocity = Vector3(forward, 0.0f, 0.0f);
		if (keyState[SDL_SCANCODE_D]) {
			velocity = Vector3(forward, sideway, 0.0f);
		}
		if (keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, -1 * sideway, 0.0f);
		}
		if (keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, 0.0f, 0.0f);
		}
		if (!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, 0.0f, 0.0f);
		}
	}
	if (!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S]) {
		velocity = Vector3(forward, 0.0f, 0.0f);
		if (keyState[SDL_SCANCODE_D]) {
			velocity = Vector3(forward, sideway, 0.0f);
		}
		if (keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, -1 * sideway, 0.0f);
		}
		if (keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, 0.0f, 0.0f);
		}
		if (!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A]) {
			velocity = Vector3(forward, 0.0f, 0.0f);
		}
	}
    if (keyState[SDL_SCANCODE_SPACE] && mSpacePressed == false) {
		mSpacePressed = true;
		Bullet* temp = new Bullet(mOwner->mGame);
		temp->SetPosition(mOwner->GetPosition());
		Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/Shoot.wav"), 0);
	}
	if (!keyState[SDL_SCANCODE_SPACE]) {
		mSpacePressed = false;
	}
}

void PlayerMove::Update(float deltaTime) {
	tracker += deltaTime;
	if (tracker >= 10.0f) {
		multiplier += .15;
		tracker = 0.0f;
	}
	//update position
	mOwner->SetPosition(mOwner->GetPosition() + (deltaTime * velocity));
	if (mOwner->GetPosition().y < -195.0f) {
		mOwner->SetPosition(Vector3(mOwner->GetPosition().x, -195.0f, mOwner->GetPosition().z));
	}
	if (mOwner->GetPosition().y > 195.0f) {
		mOwner->SetPosition(Vector3(mOwner->GetPosition().x, 195.0f, mOwner->GetPosition().z));
	}
	if (mOwner->GetPosition().z < -225.0f) {
		mOwner->SetPosition(Vector3(mOwner->GetPosition().x, mOwner->GetPosition().y, -225.0f));
	}
	if (mOwner->GetPosition().z > 225.0f) {
		mOwner->SetPosition(Vector3(mOwner->GetPosition().x, mOwner->GetPosition().y, 225.0f));
	}

	Vector3 cameraPos = mOwner->GetPosition() - (300.0f * mOwner->GetForward()) + (Vector3::UnitZ * 100.0f);
	Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward() * 20);
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ);
	mOwner->mGame->GetRenderer()->SetViewMatrix(view);

	//create and destroy sides
	if (mOwner->GetPosition().x > wallX - 3000.0f) {
		SideBlock* first = new SideBlock(mOwner->mGame);
		first->SetPosition(Vector3(wallX, -500.0f, 0.0f));
		first->mc->SetTextureIndex(colors[index]);
		SideBlock* second = new SideBlock(mOwner->mGame);
		second->SetPosition(Vector3(wallX, 500.0f, 0.0f));
		second->mc->SetTextureIndex(colors[index]);
		wallX += 500.0f;
		if (++index > 3) {
			index = 0;
		}
	}

	if (txtNum > 20) {
		exceeds = true;
	}
	if (mOwner->GetPosition().x > mOwner->mGame->blockX - 3000.0f) {
		if (exceeds) {
			txtNum = Random::GetIntRange(1, 20);
		}
		mOwner->mGame->LoadBlocks("Assets/Blocks/" + std::to_string(txtNum) + ".txt");
		mOwner->mGame->blockX += 1000.0f;
		txtNum++;
	}

	for (unsigned int i = 0; i < mOwner->mGame->blocks.size(); i++) {
		if (test->Intersect(mOwner->mGame->blocks[i]->col)) {
			mOwner->SetState(ActorState::Paused);
			Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/ShipDie.wav"), 0);
			Mix_HaltChannel(channel);
		}
	}
}
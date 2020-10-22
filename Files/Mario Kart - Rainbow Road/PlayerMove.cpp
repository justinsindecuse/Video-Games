#include "PlayerMove.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include <string>
#include "Random.h"
#include "PlayerUI.h"
#include "HeightMap.h"
#include "Enemy.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

PlayerMove::PlayerMove(Actor* owner) : VehicleMove(owner) {
	velocity = Vector3(0.0f, 0.0f, 0.0f);
	mOwner = owner;
	mOwner->SetPosition(mOwner->mGame->mMap->CellToWorld(39, 58));
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) {
		pressed = true;
	}
	else {
		pressed = false;
	}
	if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
		turn = left;
	}
	if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
		turn = right;
	}
	if ((!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_RIGHT]) && (!keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_LEFT])) {
		turn = none;
	}
	if ((keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) && (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])) {
		turn = none;
	}
}

void PlayerMove::Update(float deltaTime) {
	VehicleMove::Update(deltaTime);
}

void PlayerMove::OnLapChange(int newLap) { 
	if (newLap == 4) {
		Mix_FadeOutChannel(mOwner->mGame->channel, 250);
		Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/FinalLap.wav"), 0);
		Mix_FadeInChannel(mOwner->mGame->channel, mOwner->mGame->GetSound("Assets/Sounds/MusicFast.ogg"), -1, 4000);
	}
	if (newLap == 5) {
		if (currentLap > mOwner->mGame->mEnemy->GetComponent<VehicleMove>()->currentLap) {
			mOwner->GetComponent<PlayerUI>()->SetRaceState(PlayerUI::RaceState::Won);
			Mix_FadeOutChannel(mOwner->mGame->channel, 250);
			Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/Won.wav"), 0);
		}
		else {
			mOwner->GetComponent<PlayerUI>()->SetRaceState(PlayerUI::RaceState::Lost);
			Mix_FadeOutChannel(mOwner->mGame->channel, 250);
			Mix_PlayChannel(-1, mOwner->mGame->GetSound("Assets/Sounds/Lost.wav"), 0);
		}
		mOwner->SetState(ActorState::Paused);
		mOwner->mGame->mEnemy->SetState(ActorState::Paused);
	}
	else {
		mOwner->GetComponent<PlayerUI>()->OnLapChange(newLap);
	}
}
//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include <iostream>
#include <SDL2/SDL_image.h>

bool Game::initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	stop = false;
	window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	thickness = 10;
	ballPos.x = 507;
	ballPos.y = 379;
	ballVelocity.x = 90;
	ballVelocity.y = 90;
	Random::Init();
	int flags = IMG_INIT_PNG;
	IMG_Init(flags);
	LoadData();
	prevTime = float(SDL_GetTicks());
	return true;
}

void Game::RunLoop() {
	while (!stop) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			//case statement
		case SDL_QUIT:
			stop = true;
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		stop = true;
	}
	std::vector<Actor*> tempActors = mActors;
	for (unsigned int counter = 0; counter < tempActors.size(); counter++) {
		tempActors[counter]->ProcessInput(state);
	}
}

void Game::UpdateGame() {
	float currTime = float(SDL_GetTicks());
	//caps fps at 60
	while (currTime <= prevTime + 16) {
		currTime = float(SDL_GetTicks());
	}
	deltaTime = (currTime - prevTime) / 1000;
	prevTime = currTime;
	//Maxes out Delta Time
	if (deltaTime > 0.033f) {
		deltaTime = 0.033f;
	}
	std::vector<class Actor*> tempActors = mActors;
	for (unsigned int counter = 0; counter < tempActors.size(); counter++) {
		tempActors[counter]->Update(deltaTime);

	}
	std::vector<class Actor*> deadActors;
	for (unsigned int counter = 0; counter < tempActors.size(); counter++) {
		if (tempActors[counter]->GetState() == ActorState::Destroy) {
			deadActors.push_back(tempActors[counter]);
		}
	}
	for (unsigned int counter = 0; counter < deadActors.size(); counter++) {
		delete deadActors[counter];
	}

	
}

void Game::GenerateOutput() {
	//fill
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	for (unsigned int i = 0; i < sprites.size(); i++) {
		if (sprites[i]->IsVisible()) {
			sprites[i]->Draw(renderer);
		}
	}

	//Presenting
	SDL_RenderPresent(renderer);

	//reset key
	arrow = "";
}

void Game::DrawRect(int a, int b, int c, int d) {
	SDL_Rect rect;
	rect.x = a;
	rect.y = b;
	rect.w = c;
	rect.h = d;
	SDL_RenderFillRect(renderer, &rect);
}

void Game::AddActor(Actor* temp) {
	mActors.push_back(temp);
}

void Game::RemoveActor(Actor* temp) {
	std::vector<class Actor*>::iterator it_vector;
	it_vector = std::find(mActors.begin(), mActors.end(), temp);
	if (it_vector != mActors.end()) {
		mActors.erase(it_vector);
	}

}

void Game::LoadData() {
	Actor* test = new Actor(this);
	SpriteComponent* sc1 = new SpriteComponent(test);
	sc1->SetTexture(GetTexture("Assets/Stars.png"));
	test->SetPosition(Vector2(512,384));
	Ship* motherShip = new Ship(this);
	SpriteComponent* sc2 = new SpriteComponent(motherShip);
	for (int i = 0; i < 10; i++) {
		Asteroid* astro = new Asteroid(this);
		AddAsteroid(astro);
	}
}

void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}
	for (auto it = Textures.begin(); it != Textures.end(); it++) {
		SDL_DestroyTexture(it->second);
	}
	Textures.clear();
}

SDL_Texture* Game::GetTexture(std::string file) {
	if (!Textures.empty()) {
		if (Textures.find(file) != Textures.end()) {
			return Textures.find(file)->second;
		}
	}
	SDL_Surface* image = IMG_Load(file.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	Textures.insert({ file, texture });
	return texture;
}

void Game::AddSprite(SpriteComponent* sprite) {
	sprites.push_back(sprite);
	std::sort(sprites.begin(), sprites.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
			return a->GetDrawOrder() < b->GetDrawOrder();
	});
}
void Game::RemoveSprite(SpriteComponent* sprite) {
	std::vector<SpriteComponent*>::iterator it_vector;
	it_vector = std::find(sprites.begin(), sprites.end(), sprite);
	if (it_vector != sprites.end()) {
		sprites.erase(it_vector);
	}
}

void Game::AddAsteroid(Asteroid* temp) {
	asteroids.push_back(temp);
}
void Game::RemoveAsteroid(Asteroid* temp) {
	std::vector<class Asteroid*>::iterator it_vector;
	it_vector = std::find(asteroids.begin(), asteroids.end(), temp);
	if (it_vector != asteroids.end()) {
		asteroids.erase(it_vector);
	}
}

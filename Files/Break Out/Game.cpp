//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Block.h"
#include "Paddle.h"
#include "Ball.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <fstream>

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
	paddleX = 512;
	paddleY = 740;
	center = Vector2(512, 384);
	bWidth = 64.0f;
	bSpace = 48.0f;
	bHeight = 32.0f;
	ballStart = Vector2(center.x, paddleY - 30);
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
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
	else {
		return;
	}
}

void Game::LoadData() {
	Actor* Walls = new Actor(this);
	SpriteComponent* wallTexture = new SpriteComponent(Walls);
	wallTexture->SetTexture(GetTexture("Assets/Background.png"));
	Walls->SetPosition(center);

	std::ifstream fileReader("Assets/Level.txt");

	if (!fileReader.is_open()) {
		SDL_Log("File Did Not Open");
	}


	std::string line;
	//Loops through file reading in lines, then loops through each line making and placing blocks where appropriate
	for (int i = 0; !fileReader.eof(); i++) {
		getline(fileReader, line);
		for (unsigned int j = 0; j < line.size(); j++) {
			if (line[j] == 'A') {
				Block* temp = createBlock('A');
				temp->SetPosition(Vector2(float(bWidth + (j * bWidth)), float(bSpace + (bHeight * i))));
			}
			else if (line[j] == 'B') {
				Block* temp = createBlock('B');
				temp->SetPosition(Vector2(float(bWidth + (j * bWidth)), float(bSpace + (bHeight * i))));
			}
			else if (line[j] == 'C') {
				Block* temp = createBlock('C');
				temp->SetPosition(Vector2(float(bWidth + (j * bWidth)), float(bSpace + (bHeight * i))));
			}
			else if (line[j] == 'D') {
				Block* temp = createBlock('D');
				temp->SetPosition(Vector2(float(bWidth + (j * bWidth)), float(bSpace + (bHeight * i))));
			}
			else if (line[j] == 'E') {
				Block* temp = createBlock('E');
				temp->SetPosition(Vector2(float(bWidth + (j * bWidth)), float(bSpace + (bHeight * i))));
			}
			else if (line[j] == 'F') {
				Block* temp = createBlock('F');
				temp->SetPosition(Vector2(float(bWidth + (j * bWidth)), float(bSpace + (bHeight * i))));
			}
		}
	}

	GamePaddle = new Paddle(this);
	GamePaddle->SetPosition(Vector2(float(paddleX), float(paddleY)));

	Ball* Ballx = new Ball(this);
	Ballx->SetPosition(ballStart);


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
	std::cout << file << std::endl;
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
	else {
		return;
	}
}

//Creates a block and sets it to the appropriate texture
Block* Game::createBlock(char x) {
	Block* Block1 = new Block(this);
	std::string file = "Assets/Block";
	file += x;
	file += ".png";
	Block1->changeTexture(file);
	return Block1;
}

void Game::RemoveBlock(Block* temp) {
	std::vector<class Block*>::iterator it_vector;
	it_vector = std::find(blocks.begin(), blocks.end(), temp);
	if (it_vector != blocks.end()) {
		blocks.erase(it_vector);
	}
	else {
		return;
	}
}
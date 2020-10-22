//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "Spawner.h"
#include "SDL2/SDL_mixer.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <fstream>
#include "TiledBGComponent.h"
#include "CSVHelper.h"
#include "Collider.h"
#include "Door.h"
#include "SecretBlock.h"
#include "Key.h"

bool Game::initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	stop = false;
	window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 448, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	thickness = 10;
	Random::Init();
	int flags = IMG_INIT_PNG;
	IMG_Init(flags);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
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
	Mix_CloseAudio();
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
	else {
		return;
	}
}

void Game::LoadData() {
	Actor* Background = new Actor(this);
	TiledBGComponent* backgroundTexture = new TiledBGComponent(Background);
	backgroundTexture->SetTexture(GetTexture("Assets/Dungeon/DungeonTiles.png"));
	backgroundTexture->LoadTileCSV("Assets/Dungeon/DungeonMapBG.csv", 32, 32);

	
	std::vector<Door*> temp;
	DrawRoom("Room1");
	DrawRoom("Room2");
	DrawRoom("Room3");
	DrawRoom("Room4");
	DrawRoom("Room5");
	DrawRoom("Room6");
	DrawRoom("Room7");

	//channel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);

}
	
void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}
	for (auto it = Textures.begin(); it != Textures.end(); it++) {
		SDL_DestroyTexture(it->second);
	}
	for (auto it = Sounds.begin(); it != Sounds.end(); it++) {
		Mix_FreeChunk(it->second);
	}
	Textures.clear();
	Sounds.clear();
}

void Game::DrawRoom(std::string room) {
	std::ifstream fileReader("Assets/Dungeon/" + room + ".csv");

	if (!fileReader.is_open()) {
		SDL_Log("File Did Not Open");
	}

	std::string line;
	std::vector<std::string> holder;
	std::vector<Door*> doors;
	std::vector<Spawner*> spawns;
	//constructs csv vector
	getline(fileReader, line);
	int i = 0;
	while (!line.empty()) {
		i++;
		holder = CSVHelper::Split(line);
		if (holder[0] == "Player") {
			player1 = new Player(this);
			player1->SetPosition(Vector2(float(std::stoi(holder[1])), float(std::stoi(holder[2]))));
		}
		else if (holder[0] == "Collider") {
			Collider* temp = new Collider(this);
			temp->SetPosition(Vector2(float(std::stoi(holder[1])) + (float(std::stoi(holder[3])/2)), float(std::stoi(holder[2])) + (float(std::stoi(holder[4]) / 2))));
			temp->setCollision(std::stoi(holder[3]), std::stoi(holder[4]));
			walls.push_back(temp);
		}
		else if (holder[0] == "Door") {
			Door* temp = new Door(this);
			temp->SetPosition(Vector2(float(std::stoi(holder[1])) + (float(std::stoi(holder[3]) / 2)), float(std::stoi(holder[2])) + (float(std::stoi(holder[4]) / 2))));
			temp->DoorSetup(holder[5], holder[6], holder[7]);
			doors.push_back(temp);
		}
		else if (holder[0] == "SecretBlock") {
			SecretBlock* temp = new SecretBlock(this);
			temp->SetPosition(Vector2(float(std::stoi(holder[1])) + 16.0f, float(std::stoi(holder[2])) + 16.0f));
			temp->initial = temp->GetPosition().y;
			blocks.push_back(temp);
		}
		else if (holder[0] != "Type") {
			Spawner* temp = new Spawner(this);
			temp->SetPosition(Vector2(float(std::stoi(holder[1])) + 16.0f, float(std::stoi(holder[2])) + 16.0f));
			temp->type = holder[0];
			spawns.push_back(temp);
		}
		getline(fileReader, line);
	}
	doorsPerRoom.insert(std::pair<std::string, std::vector<Door*> >(room, doors));
	spawnsPerRoom.insert(std::pair<std::string, std::vector<Spawner*> >(room, spawns));
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
	else {
		return;
	}
}


void Game::removeEnemy(Actor* temp) {
	std::vector<class Actor*>::iterator it_vector;
	it_vector = std::find(enemies.begin(), enemies.end(), temp);
	if (it_vector != enemies.end()) {
		enemies.erase(it_vector);
	}
	else {
		return;
	}
}

Mix_Chunk* Game::GetSound(const std::string& filename) {
	if (!Sounds.empty()) {
		if (Sounds.find(filename) != Sounds.end()) {
			return Sounds.find(filename)->second;
		}
	}
	Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
	Sounds.insert({ filename, sound });
	return sound;
}

void Game::activateSpawns() {
	for (unsigned int i = 0; i < visited.size(); i++) {
		if (visited[i] == currentRoom) {
			return;
		}
	}
	std::vector<Spawner*> holder = spawnsPerRoom.find(currentRoom)->second;
	for (unsigned int i = 0; i < holder.size(); i++) {
		if (holder[i]->type == "Key") {
			Key* temp = new Key(this);
			temp->SetPosition(holder[i]->GetPosition());
		}
	}
	visited.push_back(currentRoom);
}
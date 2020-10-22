#pragma once

#include <vector>
#include "SpriteComponent.h"
#include "SDL2/SDL.h"
#include <string>
#include <algorithm>
#include <unordered_map>
#include "SDL2/SDL_mixer.h"
#include "Random.h"

class Actor;
class Paddle;
class Block;
class Goomba;

class Game {
public:
	SDL_Point ballPos;
	SDL_Point ballVelocity;
	bool initialize();
	void Shutdown();
	void RunLoop();
	void AddActor(Actor*);
	void RemoveActor(Actor*);
	void AddSprite(SpriteComponent*);
	void RemoveSprite(SpriteComponent*);
	SDL_Texture* GetTexture(std::string file);
	int paddleX;
	int paddleY;
	std::vector<Block*> blocks;
	std::vector<Goomba*> enemies;
	void removeEnemy(Goomba* temp);
	Vector2 cameraPos;
	Actor* player1;
	Mix_Chunk* GetSound(const std::string& filename);
	int channel;

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Paddle* GamePaddle;
	std::string arrow;
	float deltaTime;
	float prevTime;
	int thickness;
	bool stop;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void DrawRect(int a, int b, int c, int d);
	std::vector<class Actor*> mActors;
	void LoadData();
	void UnloadData();
	std::unordered_map<std::string, SDL_Texture*> Textures;
	std::unordered_map<std::string, Mix_Chunk*> Sounds;
	std::vector<SpriteComponent*> sprites;
	Block* createBlock(char x);
	
	
};

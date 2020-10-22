#pragma once

#include <vector>
#include "SpriteComponent.h"
#include "SDL2/SDL.h"
#include <string>
#include <algorithm>
#include <unordered_map>
#include "Random.h"

class Actor;
class Paddle;
class Block;

class Game {
public:
	SDL_Point ballPos;
	SDL_Point ballVelocity;
	bool initialize();
	void Shutdown();
	void RunLoop();
	void AddActor(Actor*);
	void RemoveActor(Actor*);
	void RemoveBlock(Block*);
	void AddSprite(SpriteComponent*);
	void RemoveSprite(SpriteComponent*);
	SDL_Texture* GetTexture(std::string file);
	int paddleX;
	int paddleY;
	Paddle* GetPaddle() { return GamePaddle; };
	std::vector<Block*> blocks;
	Vector2 center;
	float bWidth;
	float bHeight;
	float bSpace;
	Vector2 ballStart;

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
	std::vector<SpriteComponent*> sprites;
	Block* createBlock(char x);
};

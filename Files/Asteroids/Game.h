#pragma once

#include <vector>
#include "SpriteComponent.h"
#include "SDL2/SDL.h"
#include <string>
#include <algorithm>
#include <unordered_map>
#include "Random.h"
#include "Asteroid.h"

class Actor;

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
	std::vector<Asteroid*> asteroids;
	void AddAsteroid(Asteroid* temp);
	void RemoveAsteroid(Asteroid* temp);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
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
	
};

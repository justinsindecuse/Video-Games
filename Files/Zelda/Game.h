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
class Collider;
class Door;
class SecretBlock;
class Spawner;

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
	std::vector<Actor*> enemies;
	std::vector<Collider*> walls;
	void removeEnemy(Actor* temp);
	Vector2 cameraPos;
	Actor* player1;
	Mix_Chunk* GetSound(const std::string& filename);
	int channel;
	void DrawRoom(std::string room);
	std::string currentRoom = "Room1";
	std::unordered_map<std::string, std::vector<Door*> > doorsPerRoom;
	std::unordered_map<std::string, std::vector<Spawner*> > spawnsPerRoom;
	std::vector<SecretBlock*> blocks;
	std::vector<std::string> visited;
	void activateSpawns();

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
	std::unordered_map<std::string, Mix_Chunk*> Sounds;
	std::vector<SpriteComponent*> sprites;
};
